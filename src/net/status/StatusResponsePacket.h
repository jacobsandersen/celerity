//
// Created by Jacob Andersen on 8/5/25.
//

#ifndef STATUSRESPONSEPACKET_H
#define STATUSRESPONSEPACKET_H

#include <utility>

#include "src/net/Packet.h"

namespace celerity::net::status {
struct PlayerSample {
  std::string name;
  std::string id;
};

struct StatusResponsePacket final : Packet {
  std::string version_name_;
  uint32_t version_protocol_;
  uint16_t players_max_;
  uint16_t players_online_;
  std::vector<PlayerSample> players_sample_;
  std::string description_text_;
  std::string favicon_;
  bool enforces_secure_chat_;

  StatusResponsePacket(std::string version_name, const uint32_t version_protocol, const uint16_t players_max,
                       const uint16_t players_online, const std::vector<PlayerSample> &players_sample,
                       std::string description_text, std::string favicon, const bool enforces_secure_chat)
      : version_name_(std::move(version_name)),
        version_protocol_(version_protocol),
        players_max_(players_max),
        players_online_(players_online),
        players_sample_(players_sample),
        description_text_(std::move(description_text)),
        favicon_(std::move(favicon)),
        enforces_secure_chat_(enforces_secure_chat) {}

  ByteBuffer encode() const;
};
}  // namespace celerity::net::status

#endif //STATUSRESPONSEPACKET_H
