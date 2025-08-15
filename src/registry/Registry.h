//
// Created by Jacob Andersen on 8/11/25.
//

#ifndef REGISTRY_H
#define REGISTRY_H

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

  RegistryEntry(RegistryEntry&& other) noexcept
      : entry_id_(std::move(other.entry_id_)), data_(std::move(other.data_)) {}
  RegistryEntry& operator=(RegistryEntry&& other) noexcept {
    entry_id_ = std::move(other.entry_id_);
    data_ = std::move(other.data_);
    return *this;
  }

  RegistryEntry(const RegistryEntry&) = delete;
  RegistryEntry& operator=(const RegistryEntry&) = delete;

  [[nodiscard]] Identifier get_entry_id() const;
  [[nodiscard]] const std::optional<nbt::tag::TagCompound>& get_data() const;
};

class Registry {
  Identifier registry_id_;
  std::vector<RegistryEntry> entries_;

 public:
  Registry(Identifier registry_id, std::vector<RegistryEntry> entries);

  ByteBuffer& write(ByteBuffer& buffer) const;
};
}  // namespace celerity::registry

#endif  // REGISTRY_H
