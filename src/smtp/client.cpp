// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.

#include "s2smtp/smtp/client.hpp"
#include "network/channel.hpp"

namespace s2smtp {

client_t::client_t(const end_point_t &ep, security_method_t method,
                   opt_credentials credentials, network::duration_t timeout,
                   network::ssl_protocol_t ssl_protocol)
    : channel_(init(ep, method, timeout, ssl_protocol)),
      sender_(*channel_, ep.domain, method == security_method_t::e_starttls,
              ssl_protocol, credentials) {}

void client_t::submit(const mime::message_t &msg,
                      network::duration_t command_timeout) {
  sender_.submit(msg, command_timeout);
}

network::base_channel_ptr_t
client_t::init(const end_point_t &ep, security_method_t method,
               network::duration_t timeout,
               network::ssl_protocol_t ssl_protocol) {
  return method == security_method_t::e_ssl_tls
             ? network::establish_secure_channel(ep.domain, ep.port,
                                                 ssl_protocol, timeout)
             : network::establish_channel(ep.domain, ep.port, timeout);
}

} // namespace s2smtp
