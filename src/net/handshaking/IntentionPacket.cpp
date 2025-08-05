//
// Created by Jacob Andersen on 8/5/25.
//

#include "IntentionPacket.h"

#include <absl/log/log.h>

#include "src/net/Connection.h"

namespace celerity::net::handshaking {
void IntentionPacket::handle(const IntentionPacket& packet, Connection& connection) {
  switch (packet.next_state_) {
    case ClientIntention::Status:
      connection.set_state(ConnectionState::kStatus);
      break;
    case ClientIntention::Login:
      connection.set_state(ConnectionState::kLogin);
      break;
    case ClientIntention::Transfer:
      LOG(WARNING) << "Client requested Transfer during Handshake which is not supported.";
      connection.unclean_close();
      break;
  }
}
}  // namespace celerity::net::handshaking