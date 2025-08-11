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
  TagListBuilder(icu::UnicodeString name, T first_item)
      : m_name(std::move(name)), m_list([&] {
          std::vector<std::unique_ptr<tag::Tag>> items;
          items.push_back(std::make_unique<T>(std::move(first_item)));
          return tag::TagList(first_item.get_type(), std::move(items));
        }()) {}

  static std::shared_ptr<TagListBuilder> create(T first_item) {
    auto bogus = icu::UnicodeString();
    bogus.setToBogus();

    return create(bogus, first_item);
  }

  static std::shared_ptr<TagListBuilder> create(const icu::UnicodeString& name, T first_item) {
    return std::make_shared<TagListBuilder>(name, first_item);
  }

  std::shared_ptr<TagListBuilder> add(T item) {
    m_list.add(std::make_unique<T>(item));
    return this->shared_from_this();
  }

  tag::TagList build_list() { return std::move(m_list); }

  std::unique_ptr<tag::TagList> build_list_ptr() { return std::make_unique<tag::TagList>(std::move(m_list)); }

  tag::NamedTag build_named() { return {m_name, std::move(build_list_ptr())}; }

 private:
  icu::UnicodeString m_name;
  tag::TagList m_list;
};
}  // namespace celerity::nbt

#endif
