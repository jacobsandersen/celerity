#ifndef CELERITY_NET_LOGIN_PACKETLOGINOUTDISCONNECT_H
#define CELERITY_NET_LOGIN_PACKETLOGINOUTDISCONNECT_H

#include <memory>
#include <string>
#include <utility>

#include "../../Connection.h"
#include "../../OutboundPacket.h"

namespace celerity::net::login {
class PacketLoginOutDisconnect : public OutboundPacket {
 public:
  explicit PacketLoginOutDisconnect(std::string reason) : OutboundPacket(0x00), m_reason(std::move(reason)) {}

  void write_data(ByteBuffer &buffer) override;

 private:
  std::string m_reason;
};
}  // namespace celerity::net::login

#endif
