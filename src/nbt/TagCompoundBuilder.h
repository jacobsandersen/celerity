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
  explicit TagCompoundBuilder(icu::UnicodeString name)
      : m_name(std::move(name)), m_compound(std::make_unique<tag::TagCompound>()) {}

  static std::shared_ptr<TagCompoundBuilder> create() { return create(""); }

  static std::shared_ptr<TagCompoundBuilder> create(const icu::UnicodeString& name) {
    return std::make_shared<TagCompoundBuilder>(name);
  }

  template <typename T>
    requires DerivedTag<T> && (!IsTagEnd<T>)
  std::shared_ptr<TagCompoundBuilder> add(const icu::UnicodeString& name, T item) {
    m_compound->add(name, std::make_unique<T>(item));
    return shared_from_this();
  }

  tag::NamedTag build() { return {m_name, std::move(m_compound)}; }

 private:
  icu::UnicodeString m_name;
  std::unique_ptr<tag::TagCompound> m_compound;
};
}  // namespace celerity::nbt

#endif
