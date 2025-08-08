#include "Player.h"

#include <utility>

namespace celerity::player {
const std::string& Player::get_username() const { return username_; }

std::shared_ptr<uuids::uuid> Player::get_unique_id() const { return connection_.get_unique_id(); }

net::Connection& Player::get_connection() const { return connection_; }

void Player::set_mojang_profile(std::unique_ptr<MojangProfile>& mojang_profile) {
  mojang_profile_ = std::move(mojang_profile);
}

const std::unique_ptr<MojangProfile>& Player::get_mojang_profile() const { return mojang_profile_; }

const std::unique_ptr<ClientInformation>& Player::get_client_information() const { return client_information_; }

void Player::set_client_information(std::unique_ptr<ClientInformation> client_information) {
  client_information_ = std::move(client_information);
}
}  // namespace celerity::player
