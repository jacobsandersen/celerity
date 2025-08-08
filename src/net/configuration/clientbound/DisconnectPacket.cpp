#include "DisconnectPacket.h"

#include <json/value.h>
#include <json/writer.h>

namespace celerity::net::configuration::client {
ByteBuffer DisconnectPacket::encode() const {
  ByteBuffer buf;
  Json::Value response;
  response["text"] = "Disconnected during configuration: " + reason_;
  response["bold"] = true;
  response["color"] = "#D64045";

  const Json::StreamWriterBuilder builder;
  const std::string responseStr = Json::writeString(builder, response);

  buf.write_string(responseStr);
  return buf;
}
}  // namespace celerity::net::configuration::client
