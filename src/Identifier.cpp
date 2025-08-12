//
// Created by Jacob Andersen on 8/11/25.
//

#include "Identifier.h"

namespace celerity {
const std::string& Identifier::get_namespace() const { return namespace_; }

const std::string& Identifier::get_name() const { return name_; }

std::string Identifier::to_string() const { return std::format("%s:%s", get_namespace(), get_name()); }

constexpr std::ostream& Identifier::operator<<(std::ostream& os) const {
  return os << this->to_string();
}
} // celerity