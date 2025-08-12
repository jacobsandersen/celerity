//
// Created by Jacob Andersen on 8/11/25.
//

#ifndef CELERITY_IDENTIFIER_H
#define CELERITY_IDENTIFIER_H
#include <format>
#include <string>
#include <utility>

namespace celerity {
class Identifier {
  std::string namespace_;
  std::string name_;

 public:
  Identifier(std::string ns, std::string name) : namespace_(std::move(ns)), name_(std::move(name)) {}

  [[nodiscard]] const std::string& get_namespace() const;
  [[nodiscard]] const std::string& get_name() const;
  [[nodiscard]] std::string to_string() const;

  constexpr std::ostream& operator<<(std::ostream& os) const;
};
}  // namespace celerity

#endif  // IDENTIFIER_H
