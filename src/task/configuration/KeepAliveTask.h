//
// Created by Jacob Andersen on 8/9/25.
//

#ifndef KEEPALIVETASK_H
#define KEEPALIVETASK_H

#include "src/Scheduler.h"
#include "src/net/Connection.h"

namespace celerity::task::configuration {

class KeepAliveTask final : public Task {
  net::Connection& connection_;

 public:
  explicit KeepAliveTask(net::Connection& connection) : connection_(connection) {}

  void run() const override;
};

}  // namespace celerity::task::configuration

#endif  // KEEPALIVETASK_H
