// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <chrono>

#include "s2smtp/mime/message.hpp"
#include "s2smtp/network/base_channel.hpp"
#include "s2smtp/smtp/server_reply.hpp"


namespace s2smtp {
namespace command {

// RFC 5321, SMTP Commands (https://tools.ietf.org/html/rfc5321#section-4.1)

server_reply receive_greeting(network::base_channel_t &channel,
                              network::duration_t timeout);

struct command_t {
  command_t(network::base_channel_t &channel, std::string &&data);
  server_reply execute(network::duration_t timeout);

private:
  network::base_channel_t &channel_;
  const std::string data_;
};

struct helo : command_t {
  helo(network::base_channel_t &channel, const std::string &domain);
};

struct ehlo : command_t {
  ehlo(network::base_channel_t &channel, const std::string &domain);
};

struct mail_from : command_t {
  mail_from(network::base_channel_t &channel,
            const mime::mailbox_t &reverse_path);
};

struct rcpt_to : command_t {
  rcpt_to(network::base_channel_t &channel,
          const mime::mailbox_t &forward_path);
};

struct data {
  data(network::base_channel_t &channel, const mime::message_t &msg);
  server_reply execute(network::duration_t timeout);

private:
  network::base_channel_t &channel_;
  const std::string data_;
};

struct quit : command_t {
  quit(network::base_channel_t &channel);
};

// SMTP Service Extension for Secure SMTP over Transport Layer Security
// (https://tools.ietf.org/html/rfc3207)
struct starttls {
  starttls(network::base_channel_t &channel,
           network::ssl_protocol_t ssl_protocol);
  server_reply execute(network::duration_t timeout);

private:
  network::base_channel_t &channel_;
  const std::string data_;
  const network::ssl_protocol_t ssl_protocol_;
};

// The LOGIN SASL Mechanism
// (https://tools.ietf.org/pdf/draft-murchison-sasl-login-00.pdf)
struct auth_login {
  auth_login(network::base_channel_t &channel, const std::string &login,
             const std::string &password);
  server_reply execute(network::duration_t timeout);

private:
  network::base_channel_t &channel_;
  const std::string login_data_;
  const std::string password_data_;
};

} // namespace command
} // namespace s2smtp

#endif // COMMANDS_HPP
