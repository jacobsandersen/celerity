#ifndef CELERITY_NET_LOGIN_PACKETLOGINOUTLOGINSUCCESS_H
#define CELERITY_NET_LOGIN_PACKETLOGINOUTLOGINSUCCESS_H

#include "src/net/Packet.h"
#include "src/player/MojangProfile.h"
#include "src/player/Player.h"

namespace celerity::net::login::client {
struct LoginFinishedPacket final : Packet {
  std::shared_ptr<uuids::uuid> unique_id_;
  std::string username_;
  std::vector<player::MojangProfileProperty> properties;

  explicit LoginFinishedPacket(const player::Player &player)
      : unique_id_(player.get_unique_id()),
        username_(player.get_username()),
        properties(player.get_mojang_profile()->get_properties()) {}

  [[nodiscard]] ByteBuffer encode() const;
};
}  // namespace celerity::net::login::client

#endif
