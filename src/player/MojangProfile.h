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
  MojangProfile(const uuids::uuid unique_id, std::string name, std::vector<MojangProfileProperty> properties)
      : unique_id_(unique_id), name_(std::move(name)), properties_(std::move(properties)) {}

  [[nodiscard]] const uuids::uuid &get_unique_id() const;

  [[nodiscard]] const std::string &get_name() const;

  [[nodiscard]] const std::vector<MojangProfileProperty> &get_properties() const;

 private:
  uuids::uuid unique_id_{};
  std::string name_;
  std::vector<MojangProfileProperty> properties_;
};
}  // namespace celerity::player
#endif
