//
// Created by Jacob Andersen on 7/19/25.
//

#ifndef CELERITY_NBT_TAGLISTBUILDER_H
#define CELERITY_NBT_TAGLISTBUILDER_H

#include <utility>

#include "Concepts.h"
#include "tag/NamedTag.h"
#include "tag/TagList.h"

namespace celerity::nbt {
template <typename T>
  requires DerivedTag<T> && (!IsTagEnd<T>)
class TagListBuilder : public std::enable_shared_from_this<TagListBuilder<T>> {
 public:
  explicit TagListBuilder(icu::UnicodeString name) : m_name(std::move(name)) {}

  static std::shared_ptr<TagListBuilder> create() { return create(""); }

  static std::shared_ptr<TagListBuilder> create(const icu::UnicodeString& name) {
    return std::make_shared<TagListBuilder>(name);
  }

  std::shared_ptr<TagListBuilder> add(T item) {
    m_list->add(item);
    return this->shared_from_this();
  }

  tag::NamedTag build() { return {m_name, std::move(m_list)}; }

 private:
  icu::UnicodeString m_name;
  std::unique_ptr<tag::TagList> m_list;
};
}  // namespace celerity::nbt

#endif
