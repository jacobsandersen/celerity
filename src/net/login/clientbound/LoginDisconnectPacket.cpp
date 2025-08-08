#include "LoginDisconnectPacket.h"

#include <json/value.h>
#include <json/writer.h>

#include <iostream>

namespace celerity::net::login::client {

ByteBuffer LoginDisconnectPacket::encode() const {
  ByteBuffer buf;

  Json::Value response;
  response["text"] = "Disconnected during login: " + reason_;
  response["bold"] = true;
  response["color"] = "#D64045";

  const Json::StreamWriterBuilder builder;
  const std::string responseStr = Json::writeString(builder, response);

  buf.write_string(responseStr);

  return buf;
}
}  // namespace celerity::net::login::client
