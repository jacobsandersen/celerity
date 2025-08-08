//
// Created by Jacob Andersen on 7/19/25.
//

#ifndef CELERITY_NBT_TAG_TAGINTARRAY_H
#define CELERITY_NBT_TAG_TAGINTARRAY_H

#include <utility>
#include <vector>

#include "Tag.h"

namespace celerity::nbt::tag {
class TagIntArray final : public Tag {
 public:
  explicit TagIntArray(std::vector<int32_t> data) : Tag(TagType::IntArray), m_data(std::move(data)) {}
  [[nodiscard]] std::vector<int32_t> get_ints() const { return m_data; }

 private:
  std::vector<int32_t> m_data;
};
}  // namespace celerity::nbt::tag

#endif
