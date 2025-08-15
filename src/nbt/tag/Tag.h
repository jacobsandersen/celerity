//
// Created by Jacob Andersen on 7/19/25.
//

#ifndef CELERITY_NBT_TAG_TAG_H
#define CELERITY_NBT_TAG_TAG_H

#include "TagType.h"

namespace celerity::nbt::tag {
class Tag {
public:
  explicit Tag(TagType type) : m_type(std::move(type)) {
  }

  virtual ~Tag() = default;

  [[nodiscard]] TagType get_type() const { return m_type; }



private:
  TagType m_type;
};
} // namespace celerity::nbt::tag

#endif