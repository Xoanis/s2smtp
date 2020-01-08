// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.
//
// The module contains class sender t and different helper types. 
// The sender_t gets an established communication channel and implements 
// "Commands/Replies" session in order to transmit the user's message.
// sender_t issues command sequence according to rfc 5321, rfc 3207 and
// the LOGIN SASL Mechanism draft
//
// References:
// https://tools.ietf.org/html/rfc5321
// https://tools.ietf.org/html/rfc3207
// https://tools.ietf.org/pdf/draft-murchison-sasl-login-00.pdf

#ifndef SENDER_HPP
#define SENDER_HPP

#include <exception>
#include <optional>
#include <string>

#include "s2smtp/mime/message.hpp"
#include "s2smtp/network/base_channel.hpp"
#include "s2smtp/smtp/server_reply.hpp"

namespace s2smtp {

struct end_point_t {
  std::string domain;
  uint16_t port;
};

enum class security_method_t { e_none, e_starttls, e_ssl_tls };

struct credentials_t {
  std::string user;
  std::string password;
};

using opt_credentials = std::optional<credentials_t>;

class sender_t {
public:
  sender_t(network::base_channel_t &channel, std::string domain, bool starttls,
           network::ssl_protocol_t ssl_protocol, opt_credentials credentials);

  void submit(const mime::message_t &msg, network::duration_t command_timeout);

private:
  void rcpt_to_for_address_list(const mime::address_list_t &addr_list,
                                network::duration_t timeout);
  void check_(const server_reply &reply, network::duration_t timeout);

  network::base_channel_t &channel_;
  const std::string domain_;
  bool starttls_;
  network::ssl_protocol_t ssl_protocol_;
  opt_credentials credentials_;
};

} // namespace s2smtp

#endif // SENDER_HPP
