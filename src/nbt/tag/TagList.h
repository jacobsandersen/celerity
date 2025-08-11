//
// Created by Jacob Andersen on 7/19/25.
//

#ifndef CELERITY_NBT_TAG_TAGLIST_H
#define CELERITY_NBT_TAG_TAGLIST_H

#include <utility>
#include <vector>

#include "Tag.h"
#include "TagType.h"

namespace celerity::nbt::tag {
class TagList final : public Tag {
 public:
  explicit TagList(const TagType &child_type) : TagList(child_type, {}) {}

  TagList(TagType child_type, std::vector<std::unique_ptr<Tag>> items)
      : Tag(TagType::List), m_child_type(std::move(child_type)), m_internal_list(std::move(items)) {
    for (const auto &item : items) {
      if (m_child_type != item->get_type()) {
        throw std::domain_error("Cannot build a TagList with non-homogenous inner list of Tags (" +
                                m_child_type.get_type_name() + " != " + item->get_type().get_type_name() + ")");
      }
    }
  }

  void add(std::unique_ptr<Tag> tag) {
    if (m_child_type != tag->get_type()) {
      throw std::domain_error("Cannot add " + tag->get_type().get_type_name() + " to TagList of inner type " +
                              m_child_type.get_type_name());
    }
    m_internal_list.push_back(std::move(tag));
  }

  [[nodiscard]] TagType get_child_type() const { return m_child_type; }

  [[nodiscard]] std::vector<std::unique_ptr<Tag>> &get_items() { return m_internal_list; }

 private:
  TagType m_child_type;
  std::vector<std::unique_ptr<Tag>> m_internal_list;
};
}  // namespace celerity::nbt::tag

#endif
