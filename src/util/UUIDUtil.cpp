#include "UUIDUtil.h"

#include <algorithm>

namespace celerity::util {
bool UUIDUtil::valid_undashed_uuid(std::string uuid) {
  return uuid.size() == 32 && std::ranges::all_of(uuid, [](char c) { return std::isxdigit(c); });
}

std::string UUIDUtil::canonicalize_uuid(const std::string &uuid_no_dashes) {
  if (!valid_undashed_uuid(uuid_no_dashes)) {
    throw std::invalid_argument("Invalid undashed UUID provided");
  }

  std::stringstream ss;
  ss << std::hex << std::setfill('0') << std::setw(32) << uuid_no_dashes;

  std::string canonical_uuid = uuid_no_dashes;
  canonical_uuid.insert(8, "-");
  canonical_uuid.insert(13, "-");
  canonical_uuid.insert(18, "-");
  canonical_uuid.insert(23, "-");

  return canonical_uuid;
}
}  // namespace celerity::util