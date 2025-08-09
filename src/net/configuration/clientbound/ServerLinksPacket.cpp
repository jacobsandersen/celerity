//
// Created by Jacob Andersen on 8/9/25.
//

#include "ServerLinksPacket.h"

namespace celerity::net::configuration::client {
ByteBuffer ServerLinksPacket::encode() const {
  ByteBuffer buf;

  buf.write_varint(urls_.size() + custom_urls_.size());

  for (auto& [type, url] : urls_) {
    buf.write_varint(static_cast<int32_t>(type));
    buf.write_string(url);
  }

  for (auto& [label, url] : custom_urls_) {
    buf.write_string(label);
    buf.write_string(url);
  }

  return buf;
}
}  // namespace celerity::net::configuration::client