#ifndef CELERITY_NET_LOGIN_PACKETLOGINOUTLOGINSUCCESS_H
#define CELERITY_NET_LOGIN_PACKETLOGINOUTLOGINSUCCESS_H

#include <cstdint>
#include <memory>
#include <string>
#include <utility>

#include "../../../player/MojangProfile.h"
#include "../../Connection.h"
#include "../../OutboundPacket.h"

namespace celerity::net::login {
class PacketLoginOutLoginSuccess : public OutboundPacket {
 public:
  PacketLoginOutLoginSuccess(
      uuids::uuid unique_id, std::string username,
      std::vector<player::MojangProfileProperty> properties)
      : OutboundPacket(0x02),
        m_unique_id(unique_id),
        m_username(std::move(username)),
        m_properties(std::move(properties)) {}

  void write_data(ByteBuffer &buffer) override;

 private:
  uuids::uuid m_unique_id;
  std::string m_username;
  std::vector<player::MojangProfileProperty> m_properties;
};
}  // namespace celerity::net::login

#endif
