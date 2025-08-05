#include "PacketLoginInLoginAcknowledged.h"

#include <glog/logging.h>

#include "../../configuration/out/PacketConfigurationOutFeatureFlags.h"
#include "../../configuration/out/PacketConfigurationOutKeepAlive.h"
#include "../../configuration/out/PacketConfigurationOutKnownPacks.h"

namespace celerity::net::login {
void PacketLoginInLoginAcknowledged::handle(const std::shared_ptr<Connection>& conn,
                                            const std::unique_ptr<ByteBuffer>& buffer) {
  LOG(INFO) << "Client acknowledges login, switching to configuration state";

  conn->set_state(ConnectionState::Configuration);

  LOG(INFO) << "Starting configuration keep alive";
  configuration::PacketConfigurationOutKeepAlive keep_alive;
  keep_alive.send(conn);

  LOG(INFO) << "Sending feature flags...";
  configuration::PacketConfigurationOutFeatureFlags feature_flags({"minecraft:vanilla"});
  feature_flags.send(conn);

  LOG(INFO) << "Sending server's known packs...";
  configuration::PacketConfigurationOutKnownPacks known_packs({{"minecraft", "core", "1.21.5"}});
  known_packs.send(conn);
}
}  // namespace celerity::net::login
