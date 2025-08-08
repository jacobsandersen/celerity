//
// Created by Jacob Andersen on 7/19/25.
//

#ifndef CELERITY_NBT_TAG_TAGINT_H
#define CELERITY_NBT_TAG_TAGINT_H

#include "Tag.h"

namespace celerity::nbt::tag {
class TagInt final : public Tag {
 public:
  explicit TagInt(const int16_t value) : Tag(TagType::Int), m_value(value) {}
  [[nodiscard]] int32_t get_int() const { return m_value; }

 private:
  int32_t m_value;
};
}  // namespace celerity::nbt::tag

#endif
