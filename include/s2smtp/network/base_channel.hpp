// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.
//
// The module base_channel_t class. 
// It represents an abstract communication channel with an SMTP server.

#ifndef BASE_CHANNEL_HPP
#define BASE_CHANNEL_HPP

#include <chrono>
#include <memory>
#include <string>

#include "network_error.hpp"

namespace s2smtp {
namespace network {

using duration_t = std::chrono::steady_clock::duration;

enum class ssl_protocol_t {
  e_ssl_v3 = 4,
  e_tls_v1 = 7,
  e_ssl_v23 = 10,
  e_tls_v11 = 13,
  e_tls_v12 = 16,
  e_tls_v13 = 19,
  e_tls = 22
};

class base_channel_t {
public:
  virtual ~base_channel_t() {}
  virtual void write(const std::string &data, duration_t timeout) = 0;
  virtual std::string read_until(std::string_view delim,
                                 duration_t timeout) = 0;
  virtual void upgrade_to_secure(ssl_protocol_t ssl_protocol,
                                 duration_t timeout) = 0;
};

using base_channel_ptr_t = std::unique_ptr<base_channel_t>;

} // namespace network
} // namespace s2smtp

#endif // BASE_CHANNEL_HPP
