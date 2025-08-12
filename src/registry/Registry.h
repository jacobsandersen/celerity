//
// Created by Jacob Andersen on 8/11/25.
//

#ifndef REGISTRY_H
#define REGISTRY_H

#include <deque>

#include "src/Identifier.h"
#include "src/nbt/tag/TagCompound.h"

namespace celerity {
class ByteBuffer;
}

namespace celerity::registry {
class RegistryEntry {
  Identifier entry_id_;
  std::optional<nbt::tag::TagCompound> data_;

 public:
  explicit RegistryEntry(const Identifier& entry_id);
  RegistryEntry(Identifier entry_id, std::optional<nbt::tag::TagCompound> data);

  [[nodiscard]] Identifier get_entry_id() const;
  [[nodiscard]] const std::optional<nbt::tag::TagCompound>& get_data() const;
};

class Registry {
  Identifier registry_id_;
  std::deque<RegistryEntry> entries_{};

 public:
  explicit Registry(Identifier registry_id);

  void add_entry(RegistryEntry entry);

  ByteBuffer& write(ByteBuffer& buffer) const;
};
}  // namespace celerity::registry

#endif  // REGISTRY_H
