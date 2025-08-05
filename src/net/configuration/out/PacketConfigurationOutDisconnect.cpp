#include "PacketConfigurationOutDisconnect.h"

#include <json/value.h>
#include <json/writer.h>

namespace celerity::net::configuration {
void PacketConfigurationOutDisconnect::write_data(ByteBuffer& buffer) {
  Json::Value response;
  response["text"] = "Disconnected during configuration: " + m_reason;
  response["bold"] = true;
  response["color"] = "#D64045";

  Json::StreamWriterBuilder builder;
  std::string responseStr = Json::writeString(builder, response);

  buffer.write_string(responseStr);
}
}  // namespace celerity::net::configuration
