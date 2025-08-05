#include "PacketLoginInEncryptionResponse.h"

#include <cryptopp/hex.h>
#include <glog/logging.h>
#include <json/reader.h>
#include <json/value.h>

#include "../../../MinecraftServer.h"
#include "../../../util/UUIDUtil.h"
#include "../../HttpClient.h"
#include "../out/PacketLoginOutDisconnect.h"
#include "../out/PacketLoginOutLoginSuccess.h"
#include "../out/PacketLoginOutSetCompression.h"

namespace celerity::net::login {
constexpr std::string_view SESSION_URL = "https://sessionserver.mojang.com/session/minecraft/hasJoined";

void performTwosCompliment(ByteBuffer& buffer) {
  bool carry = true;
  for (int32_t i = static_cast<int32_t>(buffer.get_data_length()) - 1; i >= 0; --i) {
    uint8_t value = buffer.peek_ubyte(i);
    uint8_t newByte = ~value & 0xff;
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
  for (char byte : hashIn) {
    buffer.write_ubyte(static_cast<uint8_t>(byte));
  }

  bool negative = buffer.peek_byte() < 0;
  if (negative) {
    performTwosCompliment(buffer);
  }

  std::string result;
  std::deque<uint8_t> data = buffer.get_data();
  auto vector_data = std::vector<uint8_t>(data.begin(), data.end());

  CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(result), false, 2, "");
  encoder.Put(vector_data.data(), vector_data.size());
  encoder.MessageEnd();

  result.erase(0, result.find_first_not_of('0'));

  if (negative) {
    result = "-" + result;
  }

  return result;
}

void PacketLoginInEncryptionResponse::handle(const std::shared_ptr<Connection>& conn,
                                             const std::unique_ptr<ByteBuffer>& buffer) {
  LOG(INFO) << "Encryption response received, beginning validation...";

  int32_t shared_secret_length = buffer->read_varint();
  std::vector<uint8_t> encrypted_shared_secret = buffer->read_ubytes(shared_secret_length);

  int32_t verify_token_length = buffer->read_varint();
  std::vector<uint8_t> encrypted_verify_token = buffer->read_ubytes(verify_token_length);

  RSAKeypair keypair = MinecraftServer::get_server()->get_rsa_keypair();

  std::vector<uint8_t> decrypted_verify_token = keypair.decrypt(encrypted_verify_token);
  decrypted_verify_token.resize(celerity::VERIFY_TOKEN_SIZE);

  std::vector<uint8_t> decrypted_shared_secret = keypair.decrypt(encrypted_shared_secret);
  decrypted_shared_secret.resize(celerity::SHARED_SECRET_SIZE);
  conn->enable_encryption(decrypted_shared_secret);

  const auto verify_token = conn->get_context_value("verify_token");
  if (!verify_token || verify_token->empty()) {
    LOG(ERROR) << "Verify token was not stored by the server. Cannot validate "
                  "this client.";
    auto pkt = PacketLoginOutDisconnect("Failed to find your verify token. Try again.");
    pkt.send(conn);
    return;
  }

  try {
    if (decrypted_verify_token != boost::any_cast<std::vector<uint8_t>>(*verify_token)) {
      throw std::runtime_error("Invalid verify token");
    }
  } catch (const std::exception& e) {
    LOG(WARNING) << "Failed to validate connection's verify token: " << e.what();

    auto pkt = PacketLoginOutDisconnect("Verify token validation failed: " + std::string(e.what()));
    pkt.send(conn);
    return;
  }

  LOG(INFO) << "OK. Building server hash for Yggdrasil authentication...";

  std::string hash;
  CryptoPP::SHA1 sha1;
  sha1.Update(decrypted_shared_secret.data(), celerity::SHARED_SECRET_SIZE);
  std::vector<uint8_t> public_key = keypair.get_der_encoded_public_key();
  sha1.Update(public_key.data(), public_key.size());
  hash.resize(sha1.DigestSize());
  sha1.Final(reinterpret_cast<CryptoPP::byte*>(&hash[0]));
  std::string finalDigest = mcHexDigest(hash);

  LOG(INFO) << "OK. Creating Yggdrasil request payload...";

  std::shared_ptr<player::Player> player = MinecraftServer::get_server()->get_player(conn->get_unique_id());
  std::map<std::string, std::string> params;
  params.insert({"username", player->get_username()});
  params.insert({"serverId", finalDigest});

  LOG(INFO) << "OK. Logging in to Mojang...";

  std::string resp_body;
  int64_t resp_code{};
  bool request_status = HttpClient::get_url(SESSION_URL, params, &resp_body, &resp_code);
  if (!request_status || resp_code != 200) {
    LOG(WARNING) << "Failed to log in to Mojang. Sending disconnection...";

    PacketLoginOutDisconnect pkt("Failed to register your session with Mojang. Try again later.");
    pkt.send(conn);
    return;
  }

  LOG(INFO) << "OK. Parsing user's profile and setting up...";

  Json::Value resp_json;
  Json::Reader reader;
  if (!reader.parse(resp_body, resp_json)) {
    PacketLoginOutDisconnect pkt("Failed to parse Mojang's response. Mojang issue? Try again later.");
    pkt.send(conn);
    return;
  }

  std::vector<player::MojangProfileProperty> properties;
  for (auto property : resp_json["properties"]) {
    std::string signature;
    if (property.isMember("signature")) {
      signature = property["signature"].asString();
    }

    properties.emplace_back(property["name"].asString(), property["value"].asString(), signature);
  }

  std::string canonicalized_unique_id = util::UUIDUtil::canonicalize_uuid(resp_json["id"].asString());

  auto maybe_unique_id = uuids::uuid::from_string(canonicalized_unique_id);
  if (!maybe_unique_id.has_value()) {
    LOG(WARNING) << "Failed to parse user's UUID from Mojang response. Sending "
                    "disconnection...";

    PacketLoginOutDisconnect pkt("Failed to parse your UUID. Try again later.");
    pkt.send(conn);
    return;
  }

  auto unique_id = maybe_unique_id.value();

  player::MojangProfile profile(unique_id, resp_json["name"].asString(), properties);

  player->set_mojang_profile(std::make_shared<player::MojangProfile>(profile));

  LOG(INFO) << "OK. Checking if we need to enable compression for the connection...";

  const auto compression_threshold =
      MinecraftServer::get_server()->get_config_manager().get_server_config().get_compression_threshold();
  if (compression_threshold > 0) {
    LOG(INFO) << "Enabling connection compression with threshold " << compression_threshold;

    PacketLoginOutSetCompression set_compression{};
    set_compression.send(conn);
    conn->enable_compression();
  }

  LOG(INFO) << "OK. Sending login success...";

  PacketLoginOutLoginSuccess resp(unique_id, player->get_username(), properties);
  resp.send(conn);
}
}  // namespace celerity::net::login
