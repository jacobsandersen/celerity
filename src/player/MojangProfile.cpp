#include "MojangProfile.h"

#include <iostream>

namespace celerity::player {
bool MojangProfileProperty::is_signed() const { return !signature.empty(); }

const uuids::uuid &MojangProfile::get_unique_id() const { return unique_id_; }

const std::string &MojangProfile::get_name() const { return name_; }

const std::vector<MojangProfileProperty> &MojangProfile::get_properties() const { return properties_; }
}  // namespace celerity::player