//
// Created by Jacob Andersen on 8/9/25.
//

#include "KeepAliveKillerTask.h"

namespace celerity::task::configuration {
void KeepAliveKillerTask::run() const {
  connection_.send_disconnection("Client did not respond to keep alive within acceptable time period");
}
}  // namespace celerity::task::configuration