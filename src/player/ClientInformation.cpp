#include "ClientInformation.h"

namespace celerity::player {
const std::string &ClientInformation::get_locale() const { return m_locale; }

void ClientInformation::set_locale(const std::string &locale) { m_locale = locale; }

int8_t ClientInformation::get_view_distance() const { return m_view_distance; }

void ClientInformation::set_view_distance(int8_t view_distance) { m_view_distance = view_distance; }

int32_t ClientInformation::get_chat_mode() const { return m_chat_mode; }

void ClientInformation::set_chat_mode(int32_t chat_mode) { m_chat_mode = chat_mode; }

bool ClientInformation::are_chat_colors_enabled() const { return m_chat_colors_enabled; }

void ClientInformation::set_chat_colors_enabled(bool chat_colors_enabled) {
  m_chat_colors_enabled = chat_colors_enabled;
}

uint8_t ClientInformation::get_displayed_skin_parts() const { return m_displayed_skin_parts; }

void ClientInformation::set_displayed_skin_parts(uint8_t displayed_skin_parts) {
  m_displayed_skin_parts = displayed_skin_parts;
}

int32_t ClientInformation::get_main_hand() const { return m_main_hand; }

void ClientInformation::set_main_hand(int32_t main_hand) { m_main_hand = main_hand; }

bool ClientInformation::is_text_filtering_enabled() const { return m_enable_text_filtering; }

void ClientInformation::set_text_filtering_enabled(bool enable_text_filtering) {
  m_enable_text_filtering = enable_text_filtering;
}

bool ClientInformation::does_allow_server_listings() const { return m_allow_server_listings; }

void ClientInformation::set_allow_server_listings(bool allow_server_listings) {
  m_allow_server_listings = allow_server_listings;
}
}  // namespace celerity::player