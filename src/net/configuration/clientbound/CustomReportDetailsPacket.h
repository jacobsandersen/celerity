//
// Created by Jacob Andersen on 8/9/25.
//

#ifndef CUSTOMREPORTDETAILSPACKET_H
#define CUSTOMREPORTDETAILSPACKET_H
#include "src/net/Packet.h"

namespace celerity::net::configuration::client {

struct CustomReportDetailsPacket final : Packet {
  std::unordered_map<std::string, std::string> details_;

  explicit CustomReportDetailsPacket(const std::unordered_map<std::string, std::string> &details) : details_(details) {}

  [[nodiscard]] ByteBuffer encode() const;
};

}  // namespace celerity::net::configuration::client

#endif  // CUSTOMREPORTDETAILSPACKET_H
