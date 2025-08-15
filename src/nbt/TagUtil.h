//
// Created by Jacob Andersen on 8/15/25.
//

#ifndef CELERITY_TAGUTIL_H
#define CELERITY_TAGUTIL_H
#include <memory>

#include "Concepts.h"

namespace celerity::nbt {
struct TagUtil {
  template <typename T> requires DerivedTag<T>
  static T* downcast(const std::unique_ptr<tag::Tag>& base_tag) {
    auto* downcasted = dynamic_cast<T*>(base_tag.get());
    if (downcasted == nullptr) {
      throw std::runtime_error(
          "Attempted to downcast tag, but did not get expected type. Malformed NBT?");
    }

    return downcasted;
  }

private:
  TagUtil() = default;
};
}

#endif //CELERITY_TAGUTIL_H