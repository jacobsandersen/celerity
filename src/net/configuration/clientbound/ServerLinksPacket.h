//
// Created by Jacob Andersen on 8/9/25.
//

#ifndef SERVERLINKSPACKET_H
#define SERVERLINKSPACKET_H
#include "src/net/Packet.h"

namespace celerity::net::configuration::client {

enum class ServerLinkType {
  BugReport,
  CommunityGuidelines,
  Support,
  Status,
  Feedback,
  Community,
  Website,
  Forums,
  News,
  Announcements
};

struct ServerLinksPacket final : Packet {
  std::unordered_map<ServerLinkType, std::string> urls_;
  std::unordered_map<std::string, std::string> custom_urls_;

  ServerLinksPacket(const std::unordered_map<ServerLinkType, std::string> &urls,
                    const std::unordered_map<std::string, std::string> &custom_urls)
      : urls_(urls), custom_urls_(custom_urls) {}

  ByteBuffer encode() const;
};

}  // namespace celerity::net::configuration::client

#endif  // SERVERLINKSPACKET_H
