//
// Created by Jacob Andersen on 8/5/25.
//

#ifndef CELERITY_UTIL_PAIRHASH_H
#define CELERITY_UTIL_PAIRHASH_H

#include <utility>

namespace celerity::util {
struct PairHash {
  template <typename T1, typename T2>
  std::size_t operator()(const std::pair<T1, T2>& p) const {
    return std::hash<T1>{}(p.first) ^ std::hash<T2>{}(p.second) << 1;
  }
};
}  // namespace celerity::util

#endif  // CELERITY_UTIL_PAIRHASH_H
