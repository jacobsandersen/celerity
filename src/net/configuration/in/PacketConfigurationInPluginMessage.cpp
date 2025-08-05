#include "PacketConfigurationInPluginMessage.h"

#include <glog/logging.h>

namespace celerity::net::configuration {
void PacketConfigurationInPluginMessage::handle(const std::shared_ptr<Connection>& conn,
                                                const std::unique_ptr<ByteBuffer>& buffer) {
  LOG(INFO) << "Received configuration plugin message...";

  const std::string channel = buffer->read_string();
  LOG(INFO) << ">> Plugin message channel: " << channel;

  if (channel == "minecraft:brand") {
    const std::string brand = buffer->read_string();
    LOG(INFO) << ">> Client brand: " << brand;
  }
}
}  // namespace celerity::net::configuration
