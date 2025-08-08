//
// Created by Jacob Andersen on 7/19/25.
//

#ifndef CELERITY_NBT_TAG_TAGFLOAT_H
#define CELERITY_NBT_TAG_TAGFLOAT_H

#include "Tag.h"

namespace celerity::nbt::tag {
class TagFloat final : public Tag {
 public:
  explicit TagFloat(const float value) : Tag(TagType::Float), m_value(value) {}
  [[nodiscard]] float get_float() const { return m_value; }

 private:
  float m_value;
};
}  // namespace celerity::nbt::tag

#endif
