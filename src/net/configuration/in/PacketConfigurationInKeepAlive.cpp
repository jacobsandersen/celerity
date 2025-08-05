#include "PacketConfigurationInKeepAlive.h"

#include <glog/logging.h>

#include "../out/PacketConfigurationOutDisconnect.h"
#include "../out/PacketConfigurationOutKeepAlive.h"

namespace celerity::net::configuration {
void PacketConfigurationInKeepAlive::handle(const std::shared_ptr<Connection>& conn,
                                            const std::unique_ptr<ByteBuffer>& buffer) {
  LOG(INFO) << "Received configuration keep alive. Checking payload...";

  if (const int64_t payload = buffer->read_be_long(); conn->get_keep_alive_payload() != payload) {
    LOG(WARNING) << "Client failed to respond with correct keep alive payload. "
                    "Sending disconnection...";

    PacketConfigurationOutDisconnect disconnect("Client responded with invalid Keep Alive");
    disconnect.send(conn);
    return;
  }

  LOG(INFO) << "OK. Restarting timer to send a new keep alive in 15 seconds...";

  conn->start_new_timer(std::chrono::seconds(15), [weak_conn = std::weak_ptr(conn)]() {
    LOG(INFO) << "Sending a new keep alive (configuration)...";

    if (const auto locked = weak_conn.lock()) {
      if (locked->get_state() != ConnectionState::Configuration) {
        LOG(WARNING) << "About to send new configuration-state keep alive, "
                        "but connection no longer in configuration state";
        return;
      }

      LOG(INFO) << "Sending new keep alive...";

      PacketConfigurationOutKeepAlive new_keep_alive;
      new_keep_alive.send(locked);
    } else {
      LOG(ERROR) << "Failed to acquire connection lock in timer callback";
    }
  });
}
}  // namespace celerity::net::configuration
