//
// Created by Jacob Andersen on 8/11/25.
//

#include "Registry.h"

#include <utility>

#include "src/ByteBuffer.h"
#include "src/nbt/NBTWriter.h"

namespace celerity::registry {
RegistryEntry::RegistryEntry(const Identifier& entry_id) : RegistryEntry(entry_id, std::nullopt) {}

RegistryEntry::RegistryEntry(Identifier entry_id, std::optional<nbt::tag::TagCompound> data)
    : entry_id_(std::move(entry_id)), data_(std::move(data)) {}

Identifier RegistryEntry::get_entry_id() const { return entry_id_; }

const std::optional<nbt::tag::TagCompound>& RegistryEntry::get_data() const { return data_; }

Registry::Registry(Identifier registry_id) : registry_id_(std::move(registry_id)) {}

void Registry::add_entry(RegistryEntry entry) { entries_.push_back(std::move(entry)); }

ByteBuffer& Registry::write(ByteBuffer& buffer) const {
  buffer.write_string(registry_id_.to_string());
  for (auto& entry : entries_) {
    buffer.write_string(entry.get_entry_id().to_string());
    const auto& data = entry.get_data();
    buffer.write_boolean(data.has_value());
    if (data.has_value()) {
      const nbt::NBTWriter writer(buffer);
      writer.write_tag(std::make_unique<nbt::tag::Tag>(*data));
    }
  }
  return buffer;
}
}  // namespace celerity::registry