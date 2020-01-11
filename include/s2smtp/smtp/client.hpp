// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.
//
// The module contains class client_t.
// client_t establishes a communication channel with an SMTP server
// and passes it to sender_t instance

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "s2smtp/network/base_channel.hpp"
#include "s2smtp/s2smtp_export.h"
#include "s2smtp/smtp/sender.hpp"

namespace s2smtp {

struct end_point_t {
  std::string domain;
  uint16_t port;
};

enum class security_method_t { e_none, e_starttls, e_ssl_tls };

class S2SMTP_EXPORT client_t {
public:
  client_t(const end_point_t &ep, security_method_t method,
           opt_credentials credentials = std::nullopt,
           network::duration_t timeout = std::chrono::seconds(30),
           network::ssl_protocol_t ssl_protocol =
               network::ssl_protocol_t::e_tls_v12);

  void submit(const mime::message_t &msg,
              network::duration_t command_timeout = std::chrono::seconds(60));

private:
  network::base_channel_ptr_t init(const end_point_t &ep,
                                   security_method_t method,
                                   network::duration_t timeout,
                                   network::ssl_protocol_t ssl_protocol);

  network::base_channel_ptr_t channel_;
  sender_t sender_;
};

} // namespace s2smtp

#endif // CLIENT_HPP
