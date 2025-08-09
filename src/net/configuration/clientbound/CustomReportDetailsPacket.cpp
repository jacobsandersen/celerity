//
// Created by Jacob Andersen on 8/9/25.
//

#include "CustomReportDetailsPacket.h"

namespace celerity::net::configuration::client {
ByteBuffer CustomReportDetailsPacket::encode() const {
  ByteBuffer buf;
  buf.write_varint(details_.size());
  for (const auto& [title, desc] : details_) {
    buf.write_string(title);
    buf.write_string(desc);
  }
  return buf;
}
}  // namespace celerity::net::configuration::client