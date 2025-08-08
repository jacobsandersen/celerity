#ifndef CELERITY_UTIL_UUIDUTIL_H
#define CELERITY_UTIL_UUIDUTIL_H

#include <uuid.h>

#include <stdexcept>
#include <string>

namespace celerity::util {
class UUIDUtil {
 private:
  UUIDUtil() = default;

 public:
  static bool valid_undashed_uuid(std::string uuid);
  static std::string canonicalize_uuid(const std::string &uuid_no_dashes);
  static uuids::uuid generate_random_uuid();
};
}  // namespace celerity::util

#endif
