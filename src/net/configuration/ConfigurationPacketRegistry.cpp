//
// Created by Jacob Andersen on 8/8/25.
//

#include "ConfigurationPacketRegistry.h"

#include "clientbound/CookieRequestPacket.h"
#include "clientbound/CustomPayloadPacket.h"
#include "clientbound/DisconnectPacket.h"
#include "clientbound/FinishConfigurationPacket.h"
#include "clientbound/KeepAlivePacket.h"
#include "clientbound/PingPacket.h"
#include "clientbound/SelectKnownPacksPacket.h"
#include "clientbound/UpdateEnabledFeaturesPacket.h"
#include "serverbound/ClientInformationPacket.h"
#include "serverbound/CustomPayloadPacket.h"
#include "serverbound/KeepAlivePacket.h"
#include "serverbound/SelectKnownPacksPacket.h"

namespace celerity::net::configuration {
template <typename T>
  requires ServerboundPacket<T>
void register_serverbound(PacketRegistry& registry) {
  registry.register_serverbound<T>(ConnectionState::kConfiguration);
}

template <typename T>
  requires ClientboundPacket<T>
void register_clientbound(PacketRegistry& registry) {
  registry.register_clientbound<T>(ConnectionState::kConfiguration);
}

void ConfigurationPacketRegistry::register_all(PacketRegistry& registry) {
  register_clientbound<client::CookieRequestPacket>(registry);
  register_clientbound<client::CustomPayloadPacket>(registry);
  register_clientbound<client::DisconnectPacket>(registry);
  register_clientbound<client::FinishConfigurationPacket>(registry);
  register_clientbound<client::KeepAlivePacket>(registry);
  register_clientbound<client::PingPacket>(registry);
  // reset chat
  // registry data
  // remove resource pack
  // add resource pack
  // store cookie
  // transfer
  register_clientbound<client::UpdateEnabledFeaturesPacket>(registry);
  // update tags
  register_clientbound<client::SelectKnownPacksPacket>(registry);
  // custom report details
  // server links
  // clear dialog
  // show dialog

  register_serverbound<server::ClientInformationPacket>(registry);
  // cookie response
  register_serverbound<server::CustomPayloadPacket>(registry);
  // ack finish config
  register_serverbound<server::KeepAlivePacket>(registry);
  // pong
  // resource pack response
  register_serverbound<server::SelectKnownPacksPacket>(registry);
  // custom click action
}
}  // namespace celerity::net::configuration