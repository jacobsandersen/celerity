//
// Created by Jacob Andersen on 7/19/25.
//

#ifndef CELERITY_NBT_TAG_TAGLONG_H
#define CELERITY_NBT_TAG_TAGLONG_H

#include "Tag.h"

namespace celerity::nbt::tag {
class TagLong final : public Tag {
 public:
  explicit TagLong(const int64_t value) : Tag(TagType::Long), m_value(value) {}
  [[nodiscard]] int64_t get_long() const { return m_value; }

 private:
  int64_t m_value;
};
}  // namespace celerity::nbt::tag

#endif
