//
// Created by Jacob Andersen on 8/9/25.
//

#ifndef KEEPALIVEKILLERTASK_H
#define KEEPALIVEKILLERTASK_H
#include "src/Scheduler.h"
#include "src/net/Connection.h"

namespace celerity::task::configuration {
class KeepAliveKillerTask final : public Task {
  net::Connection& connection_;

 public:
  explicit KeepAliveKillerTask(net::Connection& connection) : connection_(connection) {}

  void run() const override;
};
}  // namespace celerity::task::configuration

#endif  // KEEPALIVEKILLERTASK_H
