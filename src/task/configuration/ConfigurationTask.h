//
// Created by Jacob Andersen on 8/9/25.
//

#ifndef CONFIGURATIONTASK_H
#define CONFIGURATIONTASK_H
#include "src/Scheduler.h"
#include "src/net/Connection.h"

namespace celerity::task::configuration {

class ConfigurationTask final : public Task {
  net::Connection& connection_;

 public:
  explicit ConfigurationTask(net::Connection& connection) : connection_(connection) {}
  void run() const override;
};

}  // namespace celerity::task::configuration

#endif  // CONFIGURATIONTASK_H
