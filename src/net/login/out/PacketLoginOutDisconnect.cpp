#include "PacketLoginOutDisconnect.h"

#include <json/value.h>
#include <json/writer.h>

#include <iostream>

namespace celerity::net::login {
void PacketLoginOutDisconnect::write_data(ByteBuffer& buffer) {
  Json::Value response;
  response["text"] = "Disconnected during login: " + m_reason;
  response["bold"] = true;
  response["color"] = "#D64045";

  Json::StreamWriterBuilder builder;
  std::string responseStr = Json::writeString(builder, response);

  buffer.write_string(responseStr);
}
}  // namespace celerity::net::login
