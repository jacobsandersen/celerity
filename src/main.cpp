#include <absl/log/globals.h>
#include <absl/log/initialize.h>

#include "MinecraftServer.h"

void signal_handler(const int signal) {
  if (signal == SIGTERM || signal == SIGINT) {
    celerity::MinecraftServer::get_server().stop();
  }
}

int main(int argc, char* argv[]) {
  absl::SetStderrThreshold(absl::LogSeverity::kInfo);
  absl::InitializeLog();

  std::signal(SIGINT, signal_handler);
  std::signal(SIGTERM, signal_handler);

  celerity::MinecraftServer::get_server().start();

  LOG(INFO) << "Goodbye.";
}
