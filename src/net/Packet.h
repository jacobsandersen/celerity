#ifndef CELERITY_NET_PACKET_H
#define CELERITY_NET_PACKET_H

#include <concepts>

#include "src/ByteBuffer.h"

namespace celerity::net {
class Connection;

struct Packet {
  virtual ~Packet() = default;
};

template <typename T>
concept ServerboundPacket = std::is_constructible_v<T, ByteBuffer&> && std::derived_from<T, Packet> &&
                            requires(const T& packet, Connection& conn) {
                              { T::handle(packet, conn) } -> std::same_as<void>;
                            };

template <typename T>
concept ClientboundPacket = std::derived_from<T, Packet> && requires(const T& packet) {
  { packet.encode() } -> std::same_as<ByteBuffer>;
};

enum class PacketDirection { kServerbound, kClientbound };

constexpr std::ostream& operator<<(std::ostream& os, const PacketDirection& direction) {
  std::string_view name;
  switch (direction) {
    case PacketDirection::kServerbound:
      name = "Serverbound";
      break;
    case PacketDirection::kClientbound:
      name = "Clientbound";
      break;
  }
  return os << name;
}

}  // namespace celerity::net

#endif
