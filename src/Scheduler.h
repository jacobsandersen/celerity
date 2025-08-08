//
// Created by Jacob Andersen on 8/6/25.
//

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <uuid.h>

#include <boost/asio/steady_timer.hpp>

#include "util/UUIDUtil.h"

namespace celerity {
template <typename T>
concept ChronoDuration = requires {
  typename T::rep;
  typename T::period;
  requires std::is_arithmetic_v<typename T::rep>;
};

template <ChronoDuration T>
static std::chrono::seconds as_seconds(T duration) {
  return std::chrono::duration_cast<std::chrono::seconds>(duration);
}

class Scheduler {
 public:
  explicit Scheduler(const boost::asio::any_io_executor& executor) : executor_(executor) {}

  template <ChronoDuration T>
  uuids::uuid schedule_task(T delay, const std::function<void()>& task) {
    const auto id = util::UUIDUtil::generate_random_uuid();
    const auto timer = std::make_shared<boost::asio::steady_timer>(executor_, as_seconds(delay));

    std::lock_guard lock(mutex_);

    scheduled_tasks_[id] = timer;

    timer->async_wait([task = task, this, id](const boost::system::error_code& ec) {
      if (ec == boost::asio::error::operation_aborted) return;
      task();
      scheduled_tasks_.erase(id);
    });

    return id;
  }

  template <ChronoDuration T>
  uuids::uuid schedule_repeating_task(T repeat_every, const std::function<void()> task) {
    return schedule_repeating_task(0, repeat_every, task);
  }

  template <ChronoDuration InitialDelayT, ChronoDuration RepeatEveryT>
  uuids::uuid schedule_repeating_task(InitialDelayT initial_delay, RepeatEveryT repeat_every,
                                      const std::function<void()>& task) {
    const auto id = util::UUIDUtil::generate_random_uuid();
    const auto timer = std::make_shared<boost::asio::steady_timer>(executor_, as_seconds(initial_delay));

    std::lock_guard lock(mutex_);

    scheduled_tasks_[id] = timer;

    auto repeating_task = std::make_shared<std::function<void()>>();
    auto inner_handler = [this, id, repeating_task](const boost::system::error_code& ec) {
      if (ec == boost::asio::error::operation_aborted) {
        scheduled_tasks_.erase(id);
        return;
      }

      (*repeating_task)();
    };
    *repeating_task = [task = task, timer, repeat_every, inner_handler] {
      task();
      timer->expires_after(as_seconds(repeat_every));
      timer->async_wait(inner_handler);
    };

    timer->async_wait(inner_handler);

    return id;
  }

  void cancel(const uuids::uuid id) {
    std::lock_guard lock(mutex_);
    if (const auto it = scheduled_tasks_.find(id); it != scheduled_tasks_.end()) {
      it->second->cancel();
      scheduled_tasks_.erase(it);
    }
  }

  void cancel_all() {
    for (auto it = scheduled_tasks_.begin(); it != scheduled_tasks_.end();) {
      it->second->cancel();
      it = scheduled_tasks_.erase(it);
    }
  }

 private:
  std::mutex mutex_;
  const boost::asio::any_io_executor& executor_;
  std::unordered_map<uuids::uuid, std::shared_ptr<boost::asio::steady_timer>> scheduled_tasks_;
};
}  // namespace celerity

#endif  // SCHEDULER_H
