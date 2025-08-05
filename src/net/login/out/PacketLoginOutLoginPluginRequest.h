#ifndef CELERITY_NET_LOGIN_PACKETLOGINOUTLOGINPLUGINREQUEST_H
#define CELERITY_NET_LOGIN_PACKETLOGINOUTLOGINPLUGINREQUEST_H

#include <cstdint>
#include <memory>
#include <utility>

#include "../../Connection.h"
#include "../../OutboundPacket.h"

namespace celerity::net::login {
class PacketLoginOutLoginPluginRequest : public OutboundPacket {
 public:
  PacketLoginOutLoginPluginRequest(int32_t message_id, bool successful,
                                   std::vector<uint8_t> bytes)
      : OutboundPacket(0x04),
        m_message_id(message_id),
        m_successful(successful),
        m_bytes(std::move(bytes)) {}

  void write_data(ByteBuffer &buffer) override;

 private:
  int32_t m_message_id;
  bool m_successful;
  std::vector<uint8_t> m_bytes;
};
}  // namespace celerity::net::login

#endif
