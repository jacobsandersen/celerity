//
// Created by Jacob Andersen on 8/9/25.
//

#ifndef REGISTRYDATATASK_H
#define REGISTRYDATATASK_H
#include "src/Scheduler.h"
#include "src/net/Connection.h"

namespace celerity::task::configuration {

class RegistryDataTask final : public Task {
  net::Connection& connection_;

 public:
  explicit RegistryDataTask(net::Connection& connection) : connection_(connection) {}

  void run() const override;
};

}  // namespace celerity::task::configuration

#endif  // REGISTRYDATATASK_H
