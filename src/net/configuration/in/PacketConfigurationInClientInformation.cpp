#include "PacketConfigurationInClientInformation.h"

#include <glog/logging.h>

#include "../../../MinecraftServer.h"
#include "../out/PacketConfigurationOutDisconnect.h"

namespace celerity::net::configuration {
void PacketConfigurationInClientInformation::handle(const std::shared_ptr<Connection>& conn,
                                                    const std::unique_ptr<ByteBuffer>& buffer) {
  LOG(INFO) << "Received client information...";

  auto player = MinecraftServer::get_server()->get_player(conn->get_unique_id());
  if (player == nullptr) {
    PacketConfigurationOutDisconnect disconnect("Could not get Player instance for your UUID");
    disconnect.send(conn);
    return;
  }

  auto info = player->get_client_information();
  info->set_locale(buffer->read_string());
  info->set_view_distance(buffer->read_byte());
  info->set_chat_mode(buffer->read_varint());
  info->set_chat_colors_enabled(buffer->read_boolean());
  info->set_displayed_skin_parts(buffer->read_ubyte());
  info->set_main_hand(buffer->read_varint());
  info->set_text_filtering_enabled(buffer->read_boolean());
  info->set_allow_server_listings(buffer->read_boolean());

  LOG(INFO) << "Client is: Locale=" << info->get_locale() << ", ViewDistance=" << info->get_view_distance()
            << ", ChatMode=" << info->get_chat_mode();
}
}  // namespace celerity::net::configuration
