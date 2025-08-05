#include "BasicTimer.h"

#include <absl/log/log.h>

namespace celerity {
void BasicTimer::start(const std::chrono::seconds& duration,
                       std::function<void()> callback) {
  m_callback = std::move(callback);
  m_timer.expires_after(duration);
  m_timer.async_wait([this](const std::error_code& error) {
    if (!error) {
      m_callback();
    }
  });
}

void BasicTimer::cancel() { m_timer.cancel(); }
}  // namespace celerity
