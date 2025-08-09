//
// Created by Jacob Andersen on 8/9/25.
//

#ifndef CUSTOMCLICKACTIONPACKET_H
#define CUSTOMCLICKACTIONPACKET_H

#include "src/nbt/NBTReader.h"
#include "src/nbt/tag/Tag.h"
#include "src/net/Packet.h"

namespace celerity::net::configuration::server {
struct CustomClickActionPacket final : Packet {
  std::string id_;
  std::unique_ptr<nbt::tag::Tag> payload_;

  explicit CustomClickActionPacket(ByteBuffer buffer) : id_(buffer.read_string()) {
    nbt::NBTReader reader(buffer);
    payload_ = reader.read_network_tag();
  }

  static void handle(const CustomClickActionPacket& packet, Connection& connection);
};
}  // namespace celerity::net::configuration::server

#endif //CUSTOMCLICKACTIONPACKET_H
