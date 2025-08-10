//
// Created by Jacob Andersen on 8/4/25.
//

#ifndef CELERITY_NET_PACKETREGISTRY_H
#define CELERITY_NET_PACKETREGISTRY_H

#include <typeindex>

#include "ConnectionState.h"
#include "Packet.h"
#include "src/util/PairHash.h"

namespace celerity::net {
class Connection;

struct ServerboundEntry {
  std::function<std::unique_ptr<Packet>(ByteBuffer&)> factory;
  std::function<void(const Packet&, Connection&)> handler;
};

struct ClientboundEntry {
  int32_t packet_id;
  std::function<ByteBuffer(const Packet&)> encoder;
};

class PacketRegistry {
 public:
  using RegistryInitFn = void (*)(PacketRegistry&);

  static PacketRegistry& get_instance() {
    static PacketRegistry registry_;
    return registry_;
  }

  [[nodiscard]] const ServerboundEntry* get_serverbound(const ConnectionState state, const std::size_t id) const {
    const auto it = serverbound_.find(state);
    if (it == serverbound_.end() || id >= it->second.size()) return nullptr;
    return &it->second[id];
  }

  template <typename T>
    requires ClientboundPacket<T>
  [[nodiscard]] const ClientboundEntry* get_clientbound(const ConnectionState state) const {
    const auto id_itr = type_to_id_.find({state, std::type_index(typeid(T))});
    if (id_itr == type_to_id_.end()) return nullptr;

    const auto id = id_itr->second;

    const auto entry_itr = clientbound_.find(state);
    if (entry_itr == clientbound_.end() || id >= entry_itr->second.size()) return nullptr;

    return &entry_itr->second[id];
  }

  template <typename T>
    requires ServerboundPacket<T>
  void register_serverbound(const ConnectionState state) {
    serverbound_[state].emplace_back(
        [](ByteBuffer& buf) { return std::make_unique<T>(buf); },
        [](const Packet& packet, Connection& connection) { T::handle(static_cast<const T&>(packet), connection); });
  }

  template <typename T>
    requires ClientboundPacket<T>
  void register_clientbound(const ConnectionState state) {
    const auto id = static_cast<int32_t>(clientbound_[state].size());
    clientbound_[state].emplace_back(
        id, [](const Packet& packet) -> ByteBuffer { return static_cast<const T&>(packet).encode(); });
    type_to_id_[{state, std::type_index(typeid(T))}] = id;
  }

  static void add_init_function(const RegistryInitFn fn) { get_init_functions()->push_back(fn); }

 private:
  std::unordered_map<ConnectionState, std::vector<ServerboundEntry>> serverbound_{};
  std::unordered_map<ConnectionState, std::vector<ClientboundEntry>> clientbound_{};
  std::unordered_map<std::pair<ConnectionState, std::type_index>, int32_t, util::PairHash> type_to_id_{};

  PacketRegistry() {
    register_all_packets();
  }

  void register_all_packets() {
    for (const auto& init_function : *get_init_functions()) {
      init_function(*this);
    }
  }

  static std::vector<RegistryInitFn>* get_init_functions() {
    static std::vector<RegistryInitFn> fns{};
    return &fns;
  }
};
}  // namespace celerity::net

#endif  // CELERITY_NET_PACKETREGISTRY_H
