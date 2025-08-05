#include "PacketLoginOutSetCompression.h"

#include <toml11/value.hpp>

#include "../../../MinecraftServer.h"

namespace celerity::net::login {
void PacketLoginOutSetCompression::write_data(ByteBuffer& buffer) {
  const auto compression_threshold =
      MinecraftServer::get_server()->get_config_manager().get_server_config().get_compression_threshold();
  buffer.write_varint(compression_threshold);
}
}  // namespace celerity::net::login
