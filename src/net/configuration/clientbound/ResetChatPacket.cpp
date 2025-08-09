//
// Created by Jacob Andersen on 8/9/25.
//

#include "ResetChatPacket.h"

namespace celerity::net::configuration::client {
// ReSharper disable once CppMemberFunctionMayBeStatic
ByteBuffer ResetChatPacket::encode() const {
  // This packet has no payload.
  return ByteBuffer();
}
}  // namespace celerity::net::configuration::client