//
// Created by Jacob Andersen on 8/15/25.
//

#include "RegistryLoader.h"

#include <absl/log/log.h>

#include <boost/algorithm/string.hpp>
#include <filesystem>

#include "src/nbt/Json2NBTReader.h"

namespace celerity::registry {
std::vector<RegistryEntry> load_registry_entries(const std::string& ns, const std::filesystem::path& path) {
  std::vector<RegistryEntry> entries;
  for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(path)) {
    if (!dirEntry.is_regular_file()) continue;
    entries.emplace_back(Identifier(ns, dirEntry.path().filename().stem().string()),
                         nbt::Json2NBTReader(dirEntry.path()).convertToNbt());
  }
  return entries;
}

std::vector<Registry> RegistryLoader::load_registries(const std::filesystem::path& server_root) {
  const auto registry_root = server_root / "data/registry";

  std::vector<Registry> registries;
  for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(registry_root)) {
    if (const auto rel_path = std::filesystem::relative(dirEntry.path(), registry_root).string();
        dirEntry.is_directory() && rel_path.contains("/")) {
      auto id = Identifier(rel_path);
      registries.emplace_back(id, std::move(load_registry_entries(id.get_namespace(), dirEntry.path())));
    }
  }

  return registries;
}
}  // namespace celerity::registry