#include "PacketLoginInLoginPluginResponse.h"

#include "../out/PacketLoginOutLoginPluginRequest.h"

namespace celerity::net::login {
void PacketLoginInLoginPluginResponse::handle(const std::shared_ptr<Connection>& conn,
                                              const std::unique_ptr<ByteBuffer>& buffer) {
  int32_t message_id = buffer->read_varint();
  std::string channel = buffer->read_string();
  std::vector<uint8_t> bytes = buffer->read_ubytes(buffer->get_data_length());

  // TODO: check for what we need to do based on message id
}
}  // namespace celerity::net::login
