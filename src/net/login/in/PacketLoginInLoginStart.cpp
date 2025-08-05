#include "PacketLoginInLoginStart.h"

#include <glog/logging.h>
#include <uuid.h>

#include <random>

#include "../../../MinecraftServer.h"
#include "../../../SharedConstants.h"
#include "../out/PacketLoginOutDisconnect.h"
#include "../out/PacketLoginOutEncryptionRequest.h"

namespace celerity::net::login {
void PacketLoginInLoginStart::handle(const std::shared_ptr<Connection>& conn,
                                     const std::unique_ptr<ByteBuffer>& buffer) {
  LOG(INFO) << "Received login start...";

  std::string username = buffer->read_string();
  uuids::uuid unique_id = buffer->read_uuid();

  LOG(INFO) << username << " (" << uuids::to_string(unique_id) << ") is joining the server...";

  auto unique_id_ptr = std::make_shared<uuids::uuid>(unique_id);
  conn->set_unique_id(unique_id_ptr);

  player::Player player(conn, username, unique_id_ptr);
  MinecraftServer::get_server()->add_player(std::make_shared<player::Player>(player));

  LOG(INFO) << "Getting server's encoded public key...";

  std::vector<uint8_t> encoded_public_key =
      MinecraftServer::get_server()->get_rsa_keypair().get_der_encoded_public_key();

  LOG(INFO) << "Generating verify token challenge for client...";

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<uint8_t> dist(0, std::numeric_limits<uint8_t>::max());

  std::vector<uint8_t> verify_token;
  for (int i = 0; i < celerity::VERIFY_TOKEN_SIZE; i++) {
    verify_token.push_back(dist(gen));
  }

  if (!conn->set_context_value("verify_token", verify_token)) {
    LOG(ERROR) << "Failed to store verify token in connection";
    PacketLoginOutDisconnect("Failed to store verify token in connection - try again later").send(conn);
    return;
  }

  LOG(INFO) << "Sending encryption request to user's client...";

  PacketLoginOutEncryptionRequest resp("", static_cast<int32_t>(encoded_public_key.size()), encoded_public_key.data(),
                                       celerity::VERIFY_TOKEN_SIZE, verify_token.data(), true);

  resp.send(conn);
}
}  // namespace celerity::net::login
