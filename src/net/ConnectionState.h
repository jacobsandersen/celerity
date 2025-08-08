//
// Created by Jacob Andersen on 8/1/25.
//

#ifndef CELERITY_NET_CONNECTIONSTATE_H
#define CELERITY_NET_CONNECTIONSTATE_H

#include <iosfwd>
#include <string_view>

namespace celerity::net {
enum class ConnectionState { kHandshaking, kStatus, kLogin, kConfiguration, kPlay };

constexpr std::ostream& operator<<(std::ostream& os, const ConnectionState& connection_state) {
  std::string_view name;
  switch (connection_state) {
    case ConnectionState::kHandshaking:
      name = "Handshaking";
      break;
    case ConnectionState::kStatus:
      name = "Status";
      break;
    case ConnectionState::kLogin:
      name = "Login";
      break;
    case ConnectionState::kConfiguration:
      name = "Configuration";
      break;
    case ConnectionState::kPlay:
      name = "Play";
      break;
  }
  return os << name;
}
}  // namespace celerity::net
#endif  // CELERITY_NET_CONNECTIONSTATE_H
