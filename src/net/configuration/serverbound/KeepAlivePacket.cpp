#include "KeepAlivePacket.h"

#include "src/net/Connection.h"
#include "src/net/configuration/clientbound/KeepAlivePacket.h"

namespace celerity::net::configuration::server {
void KeepAlivePacket::handle(const KeepAlivePacket& packet, Connection& connection) {
  if (const auto maybe_killer_id = connection.get_context_value("keep_alive_killer")) {
    try {
      const auto killer_id = boost::any_cast<uuids::uuid>(*maybe_killer_id);
      connection.get_scheduler().cancel(killer_id);
    } catch (boost::bad_any_cast&) {
    }
  }

  const auto maybe_last_payload = connection.get_context_value("keep_alive_payload");
  if (!maybe_last_payload) {
    connection.send_disconnection("Keep alive received, but I didn't ask for it");
    return;
  }

  try {
    if (const auto last_payload = boost::any_cast<int64_t>(*maybe_last_payload); last_payload != packet.payload_) {
      connection.send_disconnection("Keep alive payload did not match last sent payload");
      return;
    }
  } catch (boost::bad_any_cast&) {
  }

  // This is a bit cursed
  connection.get_scheduler().schedule_task(std::chrono::seconds(15), [&connection] {
    const auto keep_alive_payload = std::time(nullptr);
    const auto keep_alive_killer_id = connection.get_scheduler().schedule_task(std::chrono::seconds(30), [&connection] {
      connection.send_disconnection("Client did not respond to keep alive");
    });

    connection.send_packet(client::KeepAlivePacket(keep_alive_payload));

    connection.set_context_value("keep_alive_payload", keep_alive_payload);
    connection.set_context_value("keep_alive_killer", keep_alive_killer_id);
  });
}
}  // namespace celerity::net::configuration::server
