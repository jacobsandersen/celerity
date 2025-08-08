#include "HelloPacket.h"

#include <absl/log/log.h>
#include <uuid.h>

#include <random>

#include "../clientbound/LoginDisconnectPacket.h"
#include "src/MinecraftServer.h"
#include "src/net/login/clientbound/HelloPacket.h"
#include "src/player/Player.h"
#include "src/util/RandUtil.h"

namespace celerity::net::login::server {
void HelloPacket::handle(const HelloPacket& packet, Connection& connection) {
  LOG(INFO) << "Received login start...";

  const auto [username_, unique_id_] = packet;

  LOG(INFO) << username_ << " (" << uuids::to_string(unique_id_) << ") is joining the server...";

  const auto unique_id_ptr = std::make_shared<uuids::uuid>(unique_id_);
  connection.set_unique_id(unique_id_ptr);

  const auto player = std::make_shared<player::Player>(connection, username_);
  MinecraftServer::get_server()->add_player(player);

  LOG(INFO) << "Generating verify token challenge for client...";

  std::vector<uint8_t> verify_token = util::RandUtil::generate_bytes(VERIFY_TOKEN_SIZE);
  if (!connection.set_context_value("verify_token", verify_token)) {
    LOG(ERROR) << "Failed to store verify token in connection";
    connection.send_disconnection("Server error: could not store verify token. Please try again later.");
    return;
  }

  LOG(INFO) << "Getting server's encoded public key...";

  const std::vector<uint8_t> encoded_public_key =
      MinecraftServer::get_server()->get_rsa_keypair().get_der_encoded_public_key();

  LOG(INFO) << "Sending encryption request to user's client...";

  connection.send_packet(client::HelloPacket("", static_cast<int32_t>(encoded_public_key.size()),
                                             encoded_public_key.data(), VERIFY_TOKEN_SIZE, verify_token.data(), true));
}
}  // namespace celerity::net::login::server
