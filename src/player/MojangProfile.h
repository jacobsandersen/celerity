#ifndef CELERITY_PLAYER_MOJANGPROFILE_H
#define CELERITY_PLAYER_MOJANGPROFILE_H

#include <uuid.h>

#include <string>
#include <utility>

namespace celerity::player {
struct MojangProfileProperty {
  std::string name{};
  std::string value{};
  std::string signature{};

  MojangProfileProperty(std::string name, std::string value, std::string signature)
      : name(std::move(name)), value(std::move(value)), signature(std::move(signature)) {}

  [[nodiscard]] bool is_signed() const;
};

class MojangProfile {
 public:
  MojangProfile(uuids::uuid unique_id, std::string name, std::vector<MojangProfileProperty> properties)
      : m_unique_id(unique_id), m_name(std::move(name)), m_properties(std::move(properties)) {}

  [[nodiscard]] const uuids::uuid &get_unique_id() const;

  [[nodiscard]] const std::string &get_name() const;

  [[nodiscard]] const std::vector<MojangProfileProperty> &get_properties() const;

 private:
  uuids::uuid m_unique_id{};
  std::string m_name;
  std::vector<MojangProfileProperty> m_properties;
};
}  // namespace celerity::player
#endif
