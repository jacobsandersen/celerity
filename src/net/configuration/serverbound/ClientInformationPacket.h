#ifndef CELERITY_NET_CONFIGURATION_PACKETCONFIGURATIONINCLIENTINFORMATION_H
#define CELERITY_NET_CONFIGURATION_PACKETCONFIGURATIONINCLIENTINFORMATION_H

#include "../../Packet.h"
#include "src/player/ClientInformation.h"

namespace celerity::net::configuration::server {
struct ClientInformationPacket final : Packet {
  player::ClientInformation information_;

  explicit ClientInformationPacket(ByteBuffer buffer)
      : information_(buffer.read_string(), buffer.read_byte(), static_cast<player::ChatMode>(buffer.read_varint()),
                     buffer.read_boolean(), player::DisplayedSkinParts(buffer.read_ubyte()),
                     static_cast<player::MainHand>(buffer.read_varint()), buffer.read_boolean(), buffer.read_boolean(),
                     static_cast<player::ParticleStatus>(buffer.read_varint())) {}

  static void handle(const ClientInformationPacket& packet, Connection& connection);
};
}  // namespace celerity::net::configuration::server

#endif
