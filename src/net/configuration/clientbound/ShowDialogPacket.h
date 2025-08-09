//
// Created by Jacob Andersen on 8/9/25.
//

#ifndef SHOWDIALOGPACKET_H
#define SHOWDIALOGPACKET_H
#include "src/nbt/tag/Tag.h"
#include "src/nbt/tag/TagCompound.h"
#include "src/net/Packet.h"

namespace celerity::net::configuration::client {

struct ShowDialogPacket final : Packet {
  std::unique_ptr<nbt::tag::Tag> dialog_;

  explicit ShowDialogPacket(std::unique_ptr<nbt::tag::TagCompound> dialog) : dialog_(std::move(dialog)) {}

  ByteBuffer encode() const;
};

}  // namespace celerity::net::configuration::client

#endif  // SHOWDIALOGPACKET_H
