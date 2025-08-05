#ifndef CELERITY_PLAYER_PLAYER_H
#define CELERITY_PLAYER_PLAYER_H

#include <uuid.h>

#include <utility>

#include "../net/Connection.h"
#include "ClientInformation.h"
#include "MojangProfile.h"

namespace celerity::player {
class Player {
 public:
  Player(std::shared_ptr<net::Connection> conn, std::string username, std::shared_ptr<uuids::uuid> unique_id)
      : m_connection(std::move(conn)),
        m_username(std::move(username)),
        m_unique_id(std::move(unique_id)),
        m_client_information(std::make_unique<ClientInformation>()) {}

  [[nodiscard]] const std::shared_ptr<net::Connection>& get_connection() const;

  [[nodiscard]] const std::string& get_username() const;

  [[nodiscard]] std::shared_ptr<uuids::uuid> get_unique_id() const;

  [[nodiscard]] const std::shared_ptr<MojangProfile>& get_mojang_profile() const;

  void set_mojang_profile(const std::shared_ptr<MojangProfile>& mojang_profile);

  [[nodiscard]] const std::shared_ptr<ClientInformation>& get_client_information() const;

 private:
  std::shared_ptr<net::Connection> m_connection;
  std::string m_username{};
  std::shared_ptr<uuids::uuid> m_unique_id{};
  std::shared_ptr<MojangProfile> m_mojang_profile{};
  std::shared_ptr<ClientInformation> m_client_information;
};
}  // namespace celerity::player

#endif
