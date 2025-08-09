//
// Created by Jacob Andersen on 8/8/25.
//

#include "ConfigurationPacketRegistry.h"

#include "clientbound/ClearDialogPacket.h"
#include "clientbound/CookieRequestPacket.h"
#include "clientbound/CustomPayloadPacket.h"
#include "clientbound/CustomReportDetailsPacket.h"
#include "clientbound/DisconnectPacket.h"
#include "clientbound/FinishConfigurationPacket.h"
#include "clientbound/KeepAlivePacket.h"
#include "clientbound/PingPacket.h"
#include "clientbound/RegistryDataPacket.h"
#include "clientbound/ResetChatPacket.h"
#include "clientbound/ResourcePackPopPacket.h"
#include "clientbound/ResourcePackPushPacket.h"
#include "clientbound/SelectKnownPacksPacket.h"
#include "clientbound/ServerLinksPacket.h"
#include "clientbound/ShowDialogPacket.h"
#include "clientbound/StoreCookiePacket.h"
#include "clientbound/TransferPacket.h"
#include "clientbound/UpdateEnabledFeaturesPacket.h"
#include "clientbound/UpdateTagsPacket.h"
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
  register_clientbound<client::ResetChatPacket>(registry);
  register_clientbound<client::RegistryDataPacket>(registry);
  register_clientbound<client::ResourcePackPopPacket>(registry);
  register_clientbound<client::ResourcePackPushPacket>(registry);
  register_clientbound<client::StoreCookiePacket>(registry);
  register_clientbound<client::TransferPacket>(registry);
  register_clientbound<client::UpdateEnabledFeaturesPacket>(registry);
  register_clientbound<client::UpdateTagsPacket>(registry);
  register_clientbound<client::SelectKnownPacksPacket>(registry);
  register_clientbound<client::CustomReportDetailsPacket>(registry);
  register_clientbound<client::ServerLinksPacket>(registry);
  register_clientbound<client::ClearDialogPacket>(registry);
  register_clientbound<client::ShowDialogPacket>(registry);

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