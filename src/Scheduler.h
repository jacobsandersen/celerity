//
// Created by Jacob Andersen on 8/6/25.
//

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <uuid.h>

#include <boost/asio/steady_timer.hpp>
#include <unordered_set>

#include "util/UUIDUtil.h"

namespace celerity {
struct Task {
  virtual ~Task() = default;
  virtual void run() const = 0;
};

struct BaseScheduler {
  virtual ~BaseScheduler() = default;

  /**
   * Schedule an optionally-owned one-shot task that runs as soon as possible
   * @param task the task to run
   * @param owner_id an optional owner id
   * @return the id of the task
   */
  virtual uuids::uuid run_async_task(std::unique_ptr<Task> task, const std::optional<uuids::uuid> owner_id = {}) {
    return schedule_async_task(std::chrono::milliseconds(0), std::move(task), owner_id);
  }

  /**
   * Schedule an optionally-owned one-shot task that runs as soon as possible after an initial delay
   * @param delay the delay before the task runs
   * @param task the task to run
   * @param owner_id an optional owner id
   * @return the id of the task
   */
  virtual uuids::uuid schedule_async_task(std::chrono::milliseconds delay, std::unique_ptr<Task> task,
                                          std::optional<uuids::uuid> owner_id = {}) = 0;

  /**
   * Schedule an optionally-owned repeating task that runs as soon as possible and repeats at the specified interval
   * @param interval the interval at which to repeat the task
   * @param task the task to run
   * @param owner_id an optional owner id
   * @return the id of the repeating task
   */
  virtual uuids::uuid schedule_async_repeating_task(const std::chrono::milliseconds interval,
                                                    const std::shared_ptr<Task> task,
                                                    const std::optional<uuids::uuid> owner_id = {}) {
    return schedule_async_repeating_task(std::chrono::milliseconds(0), interval, task, owner_id);
  }

  /**
   * Schedule an optionally-owned repeating task that runs as soon as possible after an initial delay and repeats at the
   * specified interval
   * @param delay the delay before the task runs
   * @param interval the interval at which to repeat the task after the initial delay
   * @param task the task to run
   * @param owner_id an optional owner id
   * @return the id of the repeating task
   */
  virtual uuids::uuid schedule_async_repeating_task(std::chrono::milliseconds delay, std::chrono::milliseconds interval,
                                                    std::shared_ptr<Task> task,
                                                    std::optional<uuids::uuid> owner_id = {}) = 0;

  /**
   * Cancel a single task by its ID
   * @param id the id of the task to cancel
   */
  virtual void cancel(uuids::uuid id) = 0;
};

class Scheduler final : public BaseScheduler, public std::enable_shared_from_this<Scheduler> {
 public:
  explicit Scheduler(boost::asio::io_context& context) : executor_(context.get_executor()) {}

  uuids::uuid schedule_async_task(std::chrono::milliseconds delay, std::unique_ptr<Task> task,
                                  std::optional<uuids::uuid> owner_id) override;

  uuids::uuid schedule_async_repeating_task(std::chrono::milliseconds delay, std::chrono::milliseconds interval,
                                            std::shared_ptr<Task> task, std::optional<uuids::uuid> owner_id) override;

  void cancel(uuids::uuid id) override;

  /**
   * Cancels all tasks owned by a particular task owner
   * This method is no-op if the ID does not refer to a registered owner
   * @param id the id of the task owner
   */
  void cancel_owner(uuids::uuid id);

  /**
   * Register a task owner with this scheduler
   * @param id the id of the owner to register
   */
  void register_owner(uuids::uuid id);

 private:
  std::mutex mutex_;
  const boost::asio::any_io_executor executor_;
  std::unordered_map<uuids::uuid, std::shared_ptr<boost::asio::steady_timer>> tasks_;
  std::unordered_set<uuids::uuid> owners_;
  std::unordered_map<uuids::uuid, std::unordered_set<uuids::uuid>> owner_tasks_;

  std::shared_ptr<boost::asio::steady_timer> pop_task_timer_locked(const uuids::uuid& id);
  void cleanup_task(uuids::uuid id, const std::optional<uuids::uuid>& owner);
};

class OwnerTrackingSchedulerProxy final : public BaseScheduler {
 public:
  explicit OwnerTrackingSchedulerProxy(const std::shared_ptr<Scheduler>& scheduler);

  ~OwnerTrackingSchedulerProxy() noexcept override;

  /**
   * Schedule a one-shot task that runs as soon as possible after an initial delay and is owned by this scheduler proxy.
   *
   * Note: If an owner is specified as a parameter to this method, that value will be ignored.
   *
   * @param delay the delay before the task runs
   * @param task the task to run
   * @return the id of the task
   */
  uuids::uuid schedule_async_task(std::chrono::milliseconds delay, std::unique_ptr<Task> task,
                                  [[maybe_unused]] std::optional<uuids::uuid> = {}) override;

  /**
   * Schedule a repeating task that runs as soon as possible after an initial delay and repeats at the
   * specified interval and is owned by this scheduler proxy.
   *
   * Note: If an owner is specified as a parameter to this method, that value will be ignored.
   *
   * @param delay the delay before the task runs
   * @param interval the interval at which to repeat the task after the initial delay
   * @param task the task to run
   * @return the id of the repeating task
   */
  uuids::uuid schedule_async_repeating_task(std::chrono::milliseconds delay, std::chrono::milliseconds interval,
                                            std::shared_ptr<Task> task,
                                            [[maybe_unused]] std::optional<uuids::uuid> = {}) override;

  void cancel(uuids::uuid id) override;

 private:
  uuids::uuid id_;
  std::shared_ptr<Scheduler> scheduler_;
};
}  // namespace celerity

#endif  // SCHEDULER_H
