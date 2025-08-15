//
// Created by Jacob Andersen on 8/15/25.
//

#ifndef CELERITY_REGISTRYLOADER_H
#define CELERITY_REGISTRYLOADER_H
#include <__filesystem/filesystem_error.h>

#include "Registry.h"

namespace celerity::registry {
class RegistryLoader {
public:
  static std::vector<Registry> load_registries(const std::filesystem::path& server_root);
private:
  RegistryLoader() = default;
};
}

#endif //CELERITY_REGISTRYLOADER_H