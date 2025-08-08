#include "UUIDUtil.h"

#include <algorithm>

namespace celerity::util {
bool UUIDUtil::valid_undashed_uuid(std::string uuid) {
  return uuid.size() == 32 && std::ranges::all_of(uuid, [](const char c) { return std::isxdigit(c); });
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

uuids::uuid UUIDUtil::generate_random_uuid() {
  static std::mt19937 generator([] {
    std::random_device rd;
    std::array<int, std::mt19937::state_size> seed_data{};
    std::ranges::generate(seed_data, std::ref(rd));
    std::seed_seq seq(seed_data.begin(), seed_data.end());
    return std::mt19937(seq);
  }());

  static uuids::uuid_random_generator gen{generator};

  return gen();
}
}  // namespace celerity::util