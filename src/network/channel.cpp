// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.

#include "channel.hpp"
#include "channel_private.hpp"

namespace s2smtp::network {

channel_t::channel_t(const std::string &domain, uint16_t port,
                     duration_t connect_timeout)
    : d(new private_channel_t(domain, port, connect_timeout)) {}

channel_t::channel_t(const std::string &domain, uint16_t port,
                     ssl_protocol_t ssl_protocol, duration_t timeout)
    : d(new private_secure_channel_t(domain, port, ssl_protocol, timeout)) {}

void channel_t::upgrade_to_secure(ssl_protocol_t ssl_protocol,
                                  duration_t timeout) {
  d.reset(new private_secure_channel_t(std::move(*d), ssl_protocol, timeout));
}

void channel_t::write(const std::string &data, duration_t timeout) {
  d->write(data, timeout);
}

std::string channel_t::read_until(std::string_view delim, duration_t timeout) {
  return d->read_until(delim, timeout);
}

base_channel_ptr_t establish_channel(const std::string &domain, uint16_t port,
                                     duration_t timeout) {
  return std::make_unique<channel_t>(domain, port, timeout);
}

base_channel_ptr_t establish_secure_channel(const std::string &domain,
                                            uint16_t port,
                                            ssl_protocol_t ssl_protocol,
                                            duration_t timeout) {
  return std::make_unique<channel_t>(domain, port, ssl_protocol, timeout);
}

} // namespace s2smtp::network
