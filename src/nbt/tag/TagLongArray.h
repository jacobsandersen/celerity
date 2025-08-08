//
// Created by Jacob Andersen on 7/19/25.
//

#ifndef CELERITY_NBT_TAG_TAGLONGARRAY_H
#define CELERITY_NBT_TAG_TAGLONGARRAY_H

#include <vector>

#include "Tag.h"

namespace celerity::nbt::tag {
class TagLongArray final : public Tag {
 public:
  explicit TagLongArray(std::vector<int64_t> data) : Tag(TagType::LongArray), m_data(std::move(data)) {}
  [[nodiscard]] std::vector<int64_t> get_longs() const { return m_data; }

 private:
  std::vector<int64_t> m_data{};
};
}  // namespace celerity::nbt::tag

#endif
