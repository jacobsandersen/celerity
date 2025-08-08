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
  Player(net::Connection& conn, std::string username) : connection_(conn), username_(std::move(username)) {}

  [[nodiscard]] net::Connection& get_connection() const;

  [[nodiscard]] const std::string& get_username() const;

  [[nodiscard]] std::shared_ptr<uuids::uuid> get_unique_id() const;

  void set_mojang_profile(std::unique_ptr<MojangProfile>& mojang_profile);

  [[nodiscard]] const std::unique_ptr<MojangProfile>& get_mojang_profile() const;

  [[nodiscard]] const std::unique_ptr<ClientInformation>& get_client_information() const;

  void set_client_information(std::unique_ptr<ClientInformation> client_information);

 private:
  net::Connection& connection_;
  std::string username_{};
  std::unique_ptr<MojangProfile> mojang_profile_{};
  std::unique_ptr<ClientInformation> client_information_{};
};
}  // namespace celerity::player

#endif
