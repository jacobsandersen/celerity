#include "KeyPacket.h"

#include <absl/log/log.h>
#include <cryptopp/hex.h>
#include <json/reader.h>
#include <json/value.h>

#include "../../../MinecraftServer.h"
#include "../../../util/UUIDUtil.h"
#include "../../HttpClient.h"
#include "src/net/login/clientbound/LoginCompressionPacket.h"
#include "src/net/login/clientbound/LoginFinishedPacket.h"

namespace celerity::net::login::server {
constexpr std::string_view SESSION_URL = "https://sessionserver.mojang.com/session/minecraft/hasJoined";

void performTwosCompliment(ByteBuffer& buffer) {
  bool carry = true;
  for (int32_t i = static_cast<int32_t>(buffer.get_data_length()) - 1; i >= 0; --i) {
    const uint8_t value = buffer.peek_ubyte(i);
    const uint8_t newByte = ~value & 0xff;
    if (carry) {
      carry = newByte == 0xff;
      buffer.write_ubyte(carry ? 0 : newByte + 1, i);
    } else {
      buffer.write_ubyte(newByte, i);
    }
  }
}

std::string mcHexDigest(const std::string& hashIn) {
  ByteBuffer buffer;
  for (const char byte : hashIn) {
    buffer.write_ubyte(static_cast<uint8_t>(byte));
  }

  const bool negative = buffer.peek_byte() < 0;
  if (negative) {
    performTwosCompliment(buffer);
  }

  std::string result;
  std::deque<uint8_t> data = buffer.get_data();
  const auto vector_data = std::vector(data.begin(), data.end());

  CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(result), false, 2, "");
  encoder.Put(vector_data.data(), vector_data.size());
  encoder.MessageEnd();

  result.erase(0, result.find_first_not_of('0'));

  if (negative) {
    result = "-" + result;
  }

  return result;
}

void KeyPacket::handle(const KeyPacket& packet, Connection& connection) {
  LOG(INFO) << "Encryption response received, beginning validation...";

  MinecraftServer* server = MinecraftServer::get_server();
  const RSAKeypair keypair = server->get_rsa_keypair();
  std::vector<uint8_t> public_key = keypair.get_der_encoded_public_key();

  std::vector<uint8_t> decrypted_verify_token = keypair.decrypt(packet.encrypted_verify_token_);
  decrypted_verify_token.resize(VERIFY_TOKEN_SIZE);

  std::vector<uint8_t> decrypted_shared_secret = keypair.decrypt(packet.encrypted_shared_secret_);
  decrypted_shared_secret.resize(SHARED_SECRET_SIZE);

  connection.enable_encryption(decrypted_shared_secret);

  const auto verify_token = connection.get_context_value("verify_token");
  if (!verify_token || verify_token->empty()) {
    LOG(ERROR) << "Verify token was not stored by the server. Cannot validate this client.";
    connection.send_disconnection("Server error: Verify token not found. Please try again later.");
    return;
  }

  try {
    if (decrypted_verify_token != boost::any_cast<std::vector<uint8_t>>(*verify_token)) {
      throw std::runtime_error("Invalid verify token");
    }
  } catch (const std::exception& e) {
    LOG(WARNING) << "Failed to validate connection's verify token: " << e.what();
    connection.send_disconnection("Server error: Verify token did not match. Please try again later.");
    return;
  }

  LOG(INFO) << "OK. Building server hash for Yggdrasil authentication...";

  std::string hash;
  CryptoPP::SHA1 sha1;
  sha1.Update(decrypted_shared_secret.data(), SHARED_SECRET_SIZE);
  sha1.Update(public_key.data(), public_key.size());
  hash.resize(sha1.DigestSize());
  sha1.Final(reinterpret_cast<CryptoPP::byte*>(&hash[0]));
  std::string finalDigest = mcHexDigest(hash);

  // TODO: rewrite this section with Boost Beast later

  LOG(INFO) << "OK. Creating Yggdrasil request payload...";

  std::shared_ptr<player::Player> player = server->get_player(connection.get_unique_id());
  std::map<std::string, std::string> params;
  params.insert({"username", player->get_username()});
  params.insert({"serverId", finalDigest});

  LOG(INFO) << "OK. Logging in to Mojang...";

  std::string resp_body;
  int64_t resp_code{};
  if (bool request_status = HttpClient::get_url(SESSION_URL, params, &resp_body, &resp_code);
      !request_status || resp_code != 200) {
    LOG(WARNING) << "Failed to log in to Mojang. Sending disconnection...";
    connection.send_disconnection("Server error: Failed to register session with Mojang. Please try again later.");
    return;
  }

  LOG(INFO) << "OK. Parsing user's profile and setting up...";

  Json::Value resp_json;
  if (Json::Reader reader; !reader.parse(resp_body, resp_json)) {
    connection.send_disconnection(
        "Server error: Failed to parse session response from Mojang. Please try again later.");
    return;
  }

  std::vector<player::MojangProfileProperty> properties;
  for (auto property : resp_json["properties"]) {
    std::string signature{};
    if (property.isMember("signature")) {
      signature = property["signature"].asString();
    }

    properties.emplace_back(property["name"].asString(), property["value"].asString(), signature);
  }

  std::string canonicalized_unique_id = util::UUIDUtil::canonicalize_uuid(resp_json["id"].asString());
  auto maybe_unique_id = uuids::uuid::from_string(canonicalized_unique_id);
  if (!maybe_unique_id.has_value()) {
    LOG(WARNING) << "Failed to parse user's UUID from Mojang response. Sending disconnection...";
    connection.send_disconnection("Server error: Failed to parse your UUID. Please try again later.");
    return;
  }

  const auto unique_id = maybe_unique_id.value();
  auto profile = std::make_unique<player::MojangProfile>(unique_id, resp_json["name"].asString(), properties);
  player->set_mojang_profile(profile);

  LOG(INFO) << "OK. Checking if we need to enable compression for the connection...";

  if (const auto compression_threshold = server->get_config_manager().get_server_config().get_compression_threshold();
      compression_threshold > 0) {
    LOG(INFO) << "Enabling connection compression with threshold " << compression_threshold;
    connection.send_packet(client::LoginCompressionPacket(compression_threshold));
    connection.enable_compression();
  }

  LOG(INFO) << "OK. Sending login success...";
  connection.send_packet(client::LoginFinishedPacket(*player));
}
}  // namespace celerity::net::login::server
