//
// Created by Jacob Andersen on 7/4/25.
//

#include "KnownPack.h"

#include <format>

namespace celerity {
std::string KnownPack::get_namespace() const { return m_namespace; }

std::string KnownPack::get_id() const { return m_id; }

std::string KnownPack::get_version() const { return m_version; }

std::string KnownPack::to_string() const { return std::format("({}:{}, version {})", m_namespace, m_id, m_version); }
}  // namespace celerity
