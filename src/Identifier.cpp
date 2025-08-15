//
// Created by Jacob Andersen on 8/11/25.
//

#include "Identifier.h"

#include <boost/algorithm/string/find.hpp>
#include <vector>

namespace celerity {
Identifier::Identifier(std::string name) {
  const auto range = boost::algorithm::find_first(name, "/");
  if (range.empty()) {
    throw std::runtime_error("Could not create identifier from string: " + name);
  }

  namespace_ = std::string(name.begin(), range.begin());
  name_ = std::string(range.end(), name.end());
}

const std::string& Identifier::get_namespace() const { return namespace_; }

const std::string& Identifier::get_name() const { return name_; }

std::string Identifier::to_string() const { return std::format("{}:{}", get_namespace(), get_name()); }
}  // namespace celerity