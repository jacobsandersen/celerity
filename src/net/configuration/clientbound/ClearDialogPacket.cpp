//
// Created by Jacob Andersen on 8/9/25.
//

#include "ClearDialogPacket.h"

namespace celerity::net::configuration::client {
// ReSharper disable once CppMemberFunctionMayBeStatic
ByteBuffer ClearDialogPacket::encode() const {
  // This packet has no payload
  return ByteBuffer();
}
}  // namespace celerity::net::configuration::client