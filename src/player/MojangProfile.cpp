#include "MojangProfile.h"

#include <iostream>

namespace celerity::player {
bool MojangProfileProperty::is_signed() const { return !signature.empty(); }

const uuids::uuid &MojangProfile::get_unique_id() const { return m_unique_id; }

const std::string &MojangProfile::get_name() const { return m_name; }

const std::vector<MojangProfileProperty> &MojangProfile::get_properties() const { return m_properties; }
}  // namespace celerity::player