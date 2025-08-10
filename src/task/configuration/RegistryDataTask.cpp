//
// Created by Jacob Andersen on 8/9/25.
//

#include "RegistryDataTask.h"

namespace celerity::task::configuration {
void RegistryDataTask::run() const { LOG(INFO) << "Registry data task is running..."; }
}  // namespace celerity::task::configuration