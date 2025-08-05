#include "PacketConfigurationOutKeepAlive.h"

#include <glog/logging.h>

#include "PacketConfigurationOutDisconnect.h"

namespace celerity::net::configuration {
void PacketConfigurationOutKeepAlive::write_data(ByteBuffer& buffer) {
  int64_t payload = std::time(nullptr);
  buffer.write_be_long(payload);

  conn->set_context_value("keep_alive", payload);

  conn->start_new_timer(std::chrono::seconds(30), [weak_conn =
                                                       std::weak_ptr(conn)]() {
    if (const auto locked = weak_conn.lock()) {
      PacketConfigurationOutDisconnect disconnect(
          "Client did not respond to Keep Alive");
      disconnect.send(locked);
    } else {
      LOG(WARNING) << "Configuration keep alive timer fired (waiting for "
                      "client to respond), but connection was no longer valid";
    }
  });
}
}  // namespace celerity::net::configuration
