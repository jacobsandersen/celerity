// S
//  Created by Jacob Andersen on 8/5/25.
//

#include "StatusResponsePacket.h"

#include <json/value.h>
#include <json/writer.h>

namespace celerity::net::status {
ByteBuffer StatusResponsePacket::encode() const {
  Json::Value str;
  str["version"]["name"] = version_name_;
  str["version"]["protocol"] = version_protocol_;
  str["players"]["max"] = players_max_;
  str["players"]["online"] = players_online_;
  for (auto& [name, id] : players_sample_) {
    Json::Value sampleStr;
    sampleStr["name"] = name;
    sampleStr["id"] = id;
    str["players"]["sample"].append(sampleStr);
  }
  str["description"]["text"] = description_text_;
  str["favicon"] = favicon_;
  str["enforcesSecureChat"] = enforces_secure_chat_;

  const Json::StreamWriterBuilder builder;
  const std::string outStr = Json::writeString(builder, str);

  ByteBuffer buf;
  buf.write_string(outStr);

  return buf;
}
}  // namespace celerity::net::status