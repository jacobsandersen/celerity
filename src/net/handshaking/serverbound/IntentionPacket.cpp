//
// Created by Jacob Andersen on 8/5/25.
//

#include "IntentionPacket.h"

#include <absl/log/log.h>

#include "src/net/Connection.h"
#include "src/net/login/clientbound/LoginDisconnectPacket.h"

namespace celerity::net::handshaking::server {
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
      connection.set_state(ConnectionState::kLogin);
      connection.send_packet(login::client::LoginDisconnectPacket("Celerity does not support Transfers at this time"));
      connection.close();
      break;
  }
}
}  // namespace celerity::net::handshaking::server