//
// Created by Jacob Andersen on 7/4/25.
//

#ifndef CELERITY_KNOWNPACK_H
#define CELERITY_KNOWNPACK_H

#include <string>

namespace celerity {
class KnownPack final {
 public:
  KnownPack(const std::string& ns, const std::string& id, const std::string& version)
      : m_namespace(ns), m_id(id), m_version(version) {}

  [[nodiscard]] std::string get_namespace() const;
  [[nodiscard]] std::string get_id() const;
  [[nodiscard]] std::string get_version() const;
  std::string to_string() const;

 private:
  const std::string m_namespace;
  const std::string m_id;
  const std::string m_version;
};
}  // namespace celerity
#endif
