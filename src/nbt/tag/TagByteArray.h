//
// Created by Jacob Andersen on 7/19/25.
//

#ifndef CELERITY_NBT_TAG_TAGBYTEARRAY_H
#define CELERITY_NBT_TAG_TAGBYTEARRAY_H

#include <utility>
#include <vector>

#include "Tag.h"

namespace celerity::nbt::tag {
class TagByteArray final : public Tag {
  std::vector<int8_t> bytes_{};

 public:
  explicit TagByteArray(std::vector<int8_t> bytes) : Tag(TagType::ByteArray), bytes_(std::move(bytes)) {}
  [[nodiscard]] std::vector<int8_t> get_bytes() const { return bytes_; }
};
}  // namespace celerity::nbt::tag

#endif
