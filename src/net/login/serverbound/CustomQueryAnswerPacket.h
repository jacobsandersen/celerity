#ifndef CELERITY_NET_LOGIN_PACKETLOGININLOGINPLUGINRESPONSE_H
#define CELERITY_NET_LOGIN_PACKETLOGININLOGINPLUGINRESPONSE_H
#include "src/net/Packet.h"

namespace celerity::net::login::server {
struct CustomQueryAnswerPacket final : Packet {
  int32_t message_id_;
  std::vector<uint8_t> payload_{};

  explicit CustomQueryAnswerPacket(ByteBuffer buffer) : message_id_(buffer.read_varint()) {
    if (buffer.read_boolean()) {
      payload_ = buffer.read_ubytes(buffer.get_data_length());
    }
  }

  static void handle(const CustomQueryAnswerPacket& packet, Connection& connection);
};
}  // namespace celerity::net::login::server

#endif
