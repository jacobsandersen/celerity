#include "PacketLoginOutLoginSuccess.h"

namespace celerity::net::login {
void PacketLoginOutLoginSuccess::write_data(ByteBuffer &buffer) {
  if (m_username.length() > 16) {
    throw std::invalid_argument(
        "Tried to send login success with username longer than 16 characters.");
  }

  buffer.write_uuid(m_unique_id);
  buffer.write_string(m_username);
  buffer.write_varint(static_cast<int32_t>(m_properties.size()));
  for (const auto &prop : m_properties) {
    buffer.write_string(prop.name);
    buffer.write_string(prop.value);
    buffer.write_boolean(prop.is_signed());
    if (prop.is_signed()) {
      buffer.write_string(prop.signature);
    }
  }
}
}  // namespace celerity::net::login
