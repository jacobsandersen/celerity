//
// Created by Jacob Andersen on 7/28/25.
//

#ifndef CELERITY_NBT_TAG_NAMEDTAG_H
#define CELERITY_NBT_TAG_NAMEDTAG_H

#include <unicode/unistr.h>

#include <memory>
#include <utility>

#include "Tag.h"

namespace celerity::nbt::tag {
class NamedTag {
  icu::UnicodeString name_;
  std::unique_ptr<Tag> tag_;

 public:
  NamedTag(icu::UnicodeString name, std::unique_ptr<Tag> tag) : name_(std::move(name)), tag_(std::move(tag)) {}

  [[nodiscard]] icu::UnicodeString get_name() const { return name_; }
  [[nodiscard]] const std::unique_ptr<Tag>& get_tag() const { return tag_; }
};
}  // namespace celerity::nbt::tag

#endif  // CELERITY_NBT_TAG_NAMEDTAG_H
