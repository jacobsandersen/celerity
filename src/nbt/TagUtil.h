//
// Created by Jacob Andersen on 8/15/25.
//

#ifndef CELERITY_TAGUTIL_H
#define CELERITY_TAGUTIL_H
#include <memory>

#include "Concepts.h"

namespace celerity::nbt {
struct TagUtil {
  template <typename T>
    requires DerivedTag<T>
  static T* downcast(const std::unique_ptr<tag::Tag>& base_tag) {
    auto* downcasted = dynamic_cast<T*>(base_tag.get());
    if (downcasted == nullptr) {
      throw std::runtime_error("Attempted to downcast tag, but did not get expected type. Malformed NBT?");
    }

    return downcasted;
  }

  template <typename T>
    requires DerivedTag<T>
  static const T& downcast(const tag::Tag& base_tag) {
    try {
      return dynamic_cast<const T&>(base_tag);
    } catch (const std::bad_cast&) {
      throw std::runtime_error("Attempted to downcast tag ref, but cast failed. Malformed NBT?");
    }
  }

 private:
  TagUtil() = default;
};
}  // namespace celerity::nbt

#endif  // CELERITY_TAGUTIL_H