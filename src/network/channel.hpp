// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "s2smtp/network/base_channel.hpp"

namespace s2smtp {
namespace network {

class channel_t : public base_channel_t {
public:
  channel_t(const std::string &domain, uint16_t port,
            duration_t connect_timeout);
  channel_t(const std::string &domain, uint16_t port,
            ssl_protocol_t ssl_protocol, duration_t timeout);

  void upgrade_to_secure(ssl_protocol_t ssl_protocol,
                         duration_t timeout) override;

  void write(const std::string &data, duration_t timeout) override;
  std::string read_until(std::string_view delim, duration_t timeout) override;

private:
  std::unique_ptr<class private_channel_t> d;
};

std::unique_ptr<network::base_channel_t>
establish_channel(const std::string &domain, uint16_t port, duration_t timeout);

std::unique_ptr<network::base_channel_t>
establish_secure_channel(const std::string &domain, uint16_t port,
                         ssl_protocol_t ssl_protocol, duration_t timeout);

} // namespace network
} // namespace s2smtp

#endif // CHANNEL_HPP
