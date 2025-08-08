//
// Created by Jacob Andersen on 8/5/25.
//

#include "LoginPacketRegistry.h"

#include "clientbound/CookieRequestPacket.h"
#include "clientbound/CustomQueryPacket.h"
#include "clientbound/HelloPacket.h"
#include "clientbound/LoginCompressionPacket.h"
#include "clientbound/LoginDisconnectPacket.h"
#include "clientbound/LoginFinishedPacket.h"
#include "serverbound/CookieResponsePacket.h"
#include "serverbound/CustomQueryAnswerPacket.h"
#include "serverbound/HelloPacket.h"
#include "serverbound/KeyPacket.h"
#include "serverbound/LoginAcknowledgedPacket.h"

namespace celerity::net::login {
template <typename T>
  requires ServerboundPacket<T>
void register_serverbound(PacketRegistry& registry) {
  registry.register_serverbound<T>(ConnectionState::kLogin);
}

template <typename T>
  requires ClientboundPacket<T>
void register_clientbound(PacketRegistry& registry) {
  registry.register_clientbound<T>(ConnectionState::kLogin);
}

void LoginPacketRegistry::register_all(PacketRegistry& registry) {
  register_clientbound<client::LoginDisconnectPacket>(registry);
  register_clientbound<client::HelloPacket>(registry);
  register_clientbound<client::LoginFinishedPacket>(registry);
  register_clientbound<client::LoginCompressionPacket>(registry);
  register_clientbound<client::CustomQueryPacket>(registry);
  register_clientbound<client::CookieRequestPacket>(registry);

  register_serverbound<server::HelloPacket>(registry);
  register_serverbound<server::KeyPacket>(registry);
  register_serverbound<server::CustomQueryAnswerPacket>(registry);
  register_serverbound<server::LoginAcknowledgedPacket>(registry);
  register_serverbound<server::CookieResponsePacket>(registry);
}
}  // namespace celerity::net::login