//
// Created by Jacob Andersen on 8/15/25.
//

#ifndef CELERITY_JSON2NBTREADER_H
#define CELERITY_JSON2NBTREADER_H

#include <json/value.h>

#include "tag/TagCompound.h"

namespace celerity::nbt {
class Json2NBTReader {
  Json::Value json_;

 public:
  explicit Json2NBTReader(const std::string& filepath);

  [[nodiscard]] tag::TagCompound convertToNbt() const;
  [[nodiscard]] tag::TagCompound convertToNbt(const icu::UnicodeString& name) const;
};
}  // namespace celerity::nbt

#endif  // CELERITY_JSON2NBTREADER_H