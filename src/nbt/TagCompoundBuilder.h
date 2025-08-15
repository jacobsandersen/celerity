//
// Created by Jacob Andersen on 7/19/25.
//

#ifndef CELERITY_NBT_TAGCOMPOUNDBUILDER_H
#define CELERITY_NBT_TAGCOMPOUNDBUILDER_H

#include <memory>
#include <utility>

#include "Concepts.h"
#include "tag/NamedTag.h"
#include "tag/TagCompound.h"

namespace celerity::nbt {
class TagCompoundBuilder : public std::enable_shared_from_this<TagCompoundBuilder> {
 public:
  explicit TagCompoundBuilder(icu::UnicodeString name) : m_name(std::move(name)), m_compound(tag::TagCompound()) {}

  static std::shared_ptr<TagCompoundBuilder> create() {
    auto bogus = icu::UnicodeString();
    bogus.setToBogus();

    return create(bogus);
  }

  static std::shared_ptr<TagCompoundBuilder> create(const icu::UnicodeString& name) {
    return std::make_shared<TagCompoundBuilder>(name);
  }

  template <typename T>
    requires DerivedTag<T> && (!IsTagEnd<T>)
  std::shared_ptr<TagCompoundBuilder> add(const icu::UnicodeString& name, T item) {
    m_compound.add(name, std::make_unique<T>(std::move(item)));
    return shared_from_this();
  }

  std::shared_ptr<TagCompoundBuilder> add(const icu::UnicodeString& name, std::unique_ptr<tag::Tag> item) {
    if (item == nullptr || item->get_type() == tag::TagType::End) {
      throw std::runtime_error("Can't add null or TagEnd explicitly to TagCompound; it will be added automatically");
    }

    m_compound.add(name, std::move(item));
    return shared_from_this();
  }

  tag::TagCompound build_compound() { return std::move(m_compound); }

  std::unique_ptr<tag::TagCompound> build_compound_ptr() {
    return std::make_unique<tag::TagCompound>(std::move(m_compound));
  }

  tag::NamedTag build_named() { return {m_name, std::move(build_compound_ptr())}; }

 private:
  icu::UnicodeString m_name;
  tag::TagCompound m_compound;
};
}  // namespace celerity::nbt

#endif