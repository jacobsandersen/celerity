#ifndef CELERITY_BASICTIMER_H
#define CELERITY_BASICTIMER_H

#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>

namespace celerity {
class BasicTimer {
 public:
  explicit BasicTimer(const boost::asio::any_io_executor& executor) : m_timer(executor) {}

  void start(const std::chrono::seconds& duration, std::function<void()> callback);
  void cancel();

 private:
  boost::asio::steady_timer m_timer;
  std::function<void()> m_callback;
};
}  // namespace celerity

#endif
