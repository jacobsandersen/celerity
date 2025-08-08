//
// Created by Jacob Andersen on 8/5/25.
//

#ifndef CELERITY_NET_LOGIN_LOGINPACKETREGISTRY_H
#define CELERITY_NET_LOGIN_LOGINPACKETREGISTRY_H

#include "src/net/PacketRegistry.h"

namespace celerity::net::login {
struct LoginPacketRegistry {
  static void register_all(PacketRegistry& registry);
};

inline struct LoginRegistrations {
  LoginRegistrations() { PacketRegistry::add_init_function(LoginPacketRegistry::register_all); }
} _login_registration;
}  // namespace celerity::net::login

#endif  // CELERITY_NET_LOGIN_LOGINPACKETREGISTRY_H
