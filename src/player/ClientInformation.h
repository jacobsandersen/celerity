#ifndef CELERITY_PLAYER_CLIENTINFORMATION_H
#define CELERITY_PLAYER_CLIENTINFORMATION_H

#include <string>
#include <utility>

namespace celerity::player {
enum class ChatMode { Enabled, CommandsOnly, Hidden };

struct DisplayedSkinParts {
  bool cape{};
  bool jacket{};
  bool left_sleeve{};
  bool right_sleeve{};
  bool left_pants_leg{};
  bool right_pants_leg{};
  bool hat{};

  explicit DisplayedSkinParts(const uint8_t bitmask) {
    cape = bitmask & 1;
    jacket = bitmask & (1 << 1);
    left_sleeve = bitmask & (1 << 2);
    right_sleeve = bitmask & (1 << 3);
    left_pants_leg = bitmask & (1 << 4);
    right_pants_leg = bitmask & (1 << 5);
    hat = bitmask & (1 << 6);
  }
};

enum class MainHand { Left, Right };

enum class ParticleStatus { All, Decreased, Minimal };

struct ClientInformation {
  std::string locale_{};
  int8_t view_distance_{};
  ChatMode chat_mode_{};
  bool chat_colors_enabled_{};
  DisplayedSkinParts displayed_skin_parts_{0};
  MainHand main_hand_{};
  bool enable_text_filtering_{};
  bool allow_server_listings_{};
  ParticleStatus particle_status_{};

  ClientInformation(std::string locale, const int8_t view_distance, const ChatMode chat_mode,
                    const bool chat_colors_enabled, const DisplayedSkinParts& displayed_skin_parts,
                    const MainHand main_hand, const bool enable_text_filtering, const bool allow_server_listings,
                    const ParticleStatus particle_status)
      : locale_(std::move(locale)),
        view_distance_(view_distance),
        chat_mode_(chat_mode),
        chat_colors_enabled_(chat_colors_enabled),
        displayed_skin_parts_(displayed_skin_parts),
        main_hand_(main_hand),
        enable_text_filtering_(enable_text_filtering),
        allow_server_listings_(allow_server_listings),
        particle_status_(particle_status) {}

  ClientInformation(const ClientInformation& other) = default;
};
}  // namespace celerity::player

#endif
