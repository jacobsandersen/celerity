//
// Created by Jacob Andersen on 7/19/25.
//

#ifndef CELERITY_NBT_TAG_TAGBYTE_H
#define CELERITY_NBT_TAG_TAGBYTE_H

#include "Tag.h"

namespace celerity::nbt::tag {
class TagByte final : public Tag {
  int8_t byte_;

 public:
  explicit TagByte(const int8_t byte) : Tag(TagType::Byte), byte_(byte) {}
  [[nodiscard]] int8_t get_byte() const { return byte_; }
};
}  // namespace celerity::nbt::tag

#endif
