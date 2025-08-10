//
// Created by Jacob Andersen on 8/10/25.
//

#include "Scheduler.h"

#include <absl/log/log.h>

namespace celerity {
uuids::uuid Scheduler::schedule_async_task(std::chrono::milliseconds delay, std::unique_ptr<Task> task,
                                           std::optional<uuids::uuid> owner_id) {
  const auto id = util::UUIDUtil::generate_random_uuid();

  std::lock_guard lock(mutex_);

  if (owner_id && owners_.contains(*owner_id)) {
    owner_tasks_[*owner_id].insert(id);
  }

  const auto timer = std::make_shared<boost::asio::steady_timer>(executor_, delay);

  tasks_[id] = timer;

  timer->async_wait(
      [task = std::move(task), self = shared_from_this(), id, owner_id](const boost::system::error_code& ec) {
        if (ec == boost::asio::error::operation_aborted) return;
        task->run();
        self->cleanup_task(id, owner_id);
      });

  return id;
}

uuids::uuid Scheduler::schedule_async_repeating_task(std::chrono::milliseconds delay,
                                                     std::chrono::milliseconds interval, std::shared_ptr<Task> task,
                                                     std::optional<uuids::uuid> owner_id) {
  const auto id = util::UUIDUtil::generate_random_uuid();

  std::lock_guard lock(mutex_);

  if (owner_id && owners_.contains(*owner_id)) {
    owner_tasks_[*owner_id].insert(id);
  }

  const auto timer = std::make_shared<boost::asio::steady_timer>(executor_, delay);

  tasks_[id] = timer;

  auto repeating_task = std::make_shared<std::function<void()>>();

  auto inner_handler = [self = shared_from_this(), id, repeating_task, owner_id](const boost::system::error_code& ec) {
    if (ec == boost::asio::error::operation_aborted) {
      self->cleanup_task(id, owner_id);
      return;
    }

    (*repeating_task)();
  };

  *repeating_task = [task = std::move(task), owner_id, self = shared_from_this(), id, timer, interval, inner_handler] {
    task->run();

    if (owner_id && !self->owners_.contains(*owner_id)) {
      std::lock_guard lk(self->mutex_);
      self->cleanup_task(id, owner_id);
      return;
    }

    timer->expires_after(interval);
    timer->async_wait(inner_handler);
  };

  timer->async_wait(inner_handler);

  return id;
}

void Scheduler::cancel(const uuids::uuid id) {
  std::shared_ptr<boost::asio::steady_timer> timer_to_cancel;

  {
    std::lock_guard lock(mutex_);

    if (const auto it = std::ranges::find_if(owner_tasks_, [id](auto& p) { return p.second.contains(id); });
        it != owner_tasks_.end()) {
      it->second.erase(id);
      if (it->second.empty()) {
        owner_tasks_.erase(it);
      }
    }

    timer_to_cancel = pop_task_timer_locked(id);
  }

  if (timer_to_cancel) timer_to_cancel->cancel();
}

void Scheduler::cancel_owner(const uuids::uuid id) {
  LOG(INFO) << "Scheduler::cancel_owner called";

  std::vector<std::shared_ptr<boost::asio::steady_timer>> timers_to_cancel;

  {
    std::lock_guard lock(mutex_);

    if (const auto it = owner_tasks_.find(id); it != owner_tasks_.end()) {
      for (const auto& task_id : it->second) {
        if (const auto timer = pop_task_timer_locked(task_id)) {
          timers_to_cancel.push_back(std::move(timer));
        }
      }

      owner_tasks_.erase(it);
    }

    owners_.erase(id);
  }

  for (const auto& timer : timers_to_cancel) {
    timer->cancel();
  }

  LOG(INFO) << "Scheduler automatically cancelled " << timers_to_cancel.size() << " tasks";
}

void Scheduler::register_owner(const uuids::uuid id) { owners_.insert(id); }

std::shared_ptr<boost::asio::steady_timer> Scheduler::pop_task_timer_locked(const uuids::uuid& id) {
  if (const auto it = tasks_.find(id); it != tasks_.end()) {
    auto timer = it->second;
    tasks_.erase(it);
    return timer;
  }

  return nullptr;
}

void Scheduler::cleanup_task(const uuids::uuid id, const std::optional<uuids::uuid>& owner) {
  std::lock_guard lock(mutex_);
  tasks_.erase(id);
  if (owner) {
    if (const auto it = owner_tasks_.find(*owner); it != owner_tasks_.end()) {
      it->second.erase(id);
      if (it->second.empty()) {
        owner_tasks_.erase(it);
      }
    }
  }
}

OwnerTrackingSchedulerProxy::OwnerTrackingSchedulerProxy(const std::shared_ptr<Scheduler>& scheduler)
    : id_(util::UUIDUtil::generate_random_uuid()), scheduler_(scheduler) {
  scheduler_->register_owner(id_);
}

OwnerTrackingSchedulerProxy::~OwnerTrackingSchedulerProxy() noexcept {
  LOG(INFO) << "Owner tracking scheduler proxy is destructing, cancelling all owned tasks";
  scheduler_->cancel_owner(id_);
}

uuids::uuid OwnerTrackingSchedulerProxy::schedule_async_task(const std::chrono::milliseconds delay,
                                                             std::unique_ptr<Task> task, std::optional<uuids::uuid>) {
  return scheduler_->schedule_async_task(delay, std::move(task), {id_});
}

uuids::uuid OwnerTrackingSchedulerProxy::schedule_async_repeating_task(const std::chrono::milliseconds delay,
                                                                       const std::chrono::milliseconds interval,
                                                                       std::shared_ptr<Task> task,
                                                                       std::optional<uuids::uuid>) {
  return scheduler_->schedule_async_repeating_task(delay, interval, std::move(task), {id_});
}
void OwnerTrackingSchedulerProxy::cancel(const uuids::uuid id) {
  return scheduler_->cancel(id);
}
}  // namespace celerity