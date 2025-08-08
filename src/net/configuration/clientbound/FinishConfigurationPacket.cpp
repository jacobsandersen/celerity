//
// Created by Jacob Andersen on 8/6/25.
//

#include "FinishConfigurationPacket.h"

namespace celerity::net::configuration::client {
// ReSharper disable once CppMemberFunctionMayBeStatic
ByteBuffer FinishConfigurationPacket::encode() const {  // NOLINT(*-convert-member-functions-to-static)
  // This packet has no payload
  return {};
}
}  // namespace celerity::net::configuration::client