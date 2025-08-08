#ifndef CELERITY_NET_CONFIGURATION_PACKETCONFIGURATIONINPLUGINMESSAGE_H
#define CELERITY_NET_CONFIGURATION_PACKETCONFIGURATIONINPLUGINMESSAGE_H
#include "src/net/Packet.h"

namespace celerity::net::configuration::server {
struct CustomPayloadPacket final : Packet {
  std::string channel_;
  std::vector<uint8_t> payload_;

  explicit CustomPayloadPacket(ByteBuffer buffer) : channel_(buffer.read_string()) {
    payload_ = buffer.read_ubytes(buffer.get_data_length());
  }

  static void handle(const CustomPayloadPacket& packet, Connection& connection);
};
}  // namespace celerity::net::configuration::server

#endif
