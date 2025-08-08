//
// Created by Jacob Andersen on 8/6/25.
//

#include "RandUtil.h"

#include <random>

namespace celerity::util {
std::vector<uint8_t> RandUtil::generate_bytes(const size_t num_bytes) {
  std::vector<uint8_t> out;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<uint8_t> dist(0, std::numeric_limits<uint8_t>::max());

  for (int i = 0; i < num_bytes; i++) {
    out.push_back(dist(gen));
  }

  return out;
}
}  // namespace celerity::util