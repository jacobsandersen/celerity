//
// Created by Jacob Andersen on 8/6/25.
//

#ifndef RANDUTIL_H
#define RANDUTIL_H
#include <vector>

namespace celerity::util {

class RandUtil {
  RandUtil() = default;

 public:
  static std::vector<uint8_t> generate_bytes(size_t num_bytes);
};

}  // namespace celerity::util

#endif  // RANDUTIL_H
