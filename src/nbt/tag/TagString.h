//
// Created by Jacob Andersen on 7/19/25.
//

#ifndef CELERITY_NBT_TAG_TAGSTRING_H
#define CELERITY_NBT_TAG_TAGSTRING_H

#include "Tag.h"

namespace celerity::nbt::tag {
class TagString final : public Tag {
 public:
  explicit TagString(icu::UnicodeString value) : Tag(TagType::String), m_value(std::move(value)) {}
  [[nodiscard]] icu::UnicodeString get_string() const { return m_value; }

 private:
  icu::UnicodeString m_value;
};
}  // namespace celerity::nbt::tag

#endif
