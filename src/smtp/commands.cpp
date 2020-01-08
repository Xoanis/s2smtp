// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.

#include "commands.hpp"
#include <cppcodec/base64_rfc4648.hpp>
#include <stdexcept>

#include "parsing/smtp_parse.hpp"
#include "serialization/mime/message_serialization.hpp"
#include "serialization/smtp/commands_serialization.hpp"

namespace s2smtp {
namespace command {

const char *unexpected_error_msg =
    "Server should send positive intermediate respond after DATA command.";

server_reply receive_reply(network::base_channel_t &channel,
                           network::duration_t timeout) {
  auto r = channel.read_until("\r\n", timeout);
  if (parsing::is_begin_of_multiline_reply(r)) {
    std::string temp;
    do {
      temp = channel.read_until("\r\n", timeout);
      r += temp;
    } while (!parsing::is_end_of_multiline_reply(temp));
  }
  return parsing::parse_server_reply(r);
}

server_reply receive_greeting(network::base_channel_t &channel,
                              network::duration_t timeout) {
  return receive_reply(channel, timeout);
}

command_t::command_t(network::base_channel_t &channel, std::string &&data)
    : channel_(channel), data_(std::move(data)) {}

server_reply command_t::execute(network::duration_t timeout) {
  channel_.write(data_, timeout);
  return receive_reply(channel_, timeout);
}

data::data(network::base_channel_t &channel, const mime::message_t &msg)
    : channel_(channel), data_(serialization::serialize_data_content(msg)) {}

server_reply data::execute(network::duration_t timeout) {
  channel_.write(serialization::serialize_data(), timeout);
  auto intermediate_reply = receive_reply(channel_, timeout);

  if (intermediate_reply.kind == server_reply::kind_t::positive_completion)
    throw smtp_error(unexpected_error_msg);

  if (intermediate_reply.kind != server_reply::kind_t::positive_intermediate)
    return intermediate_reply;

  channel_.write(data_, timeout);
  return receive_reply(channel_, timeout);
}

using base64 = cppcodec::base64_rfc4648;

auth_login::auth_login(network::base_channel_t &channel,
                       const std::string &login, const std::string &password)
    : channel_(channel), login_data_(base64::encode(login) + "\r\n"),
      password_data_(base64::encode(password) + "\r\n") {}

server_reply auth_login::execute(network::duration_t timeout) {
  channel_.write("AUTH LOGIN\r\n", timeout);

  auto r1 = receive_reply(channel_, timeout);
  if (r1.kind == server_reply::kind_t::positive_completion)
    throw smtp_error(unexpected_error_msg);
  if (r1.kind != server_reply::kind_t::positive_intermediate ||
      r1.category != server_reply::category_t::auth)
    return r1;

  channel_.write(login_data_, timeout);

  auto r2 = receive_reply(channel_, timeout);
  if (r2.kind == server_reply::kind_t::positive_completion)
    throw smtp_error(unexpected_error_msg);
  if (r2.kind != server_reply::kind_t::positive_intermediate ||
      r2.category != server_reply::category_t::auth)
    return r2;

  channel_.write(password_data_, timeout);
  return receive_reply(channel_, timeout);
}

helo::helo(network::base_channel_t &channel, const std::string &domain)
    : command_t(channel, serialization::serialize_helo(domain)) {}

ehlo::ehlo(network::base_channel_t &channel, const std::string &domain)
    : command_t(channel, serialization::serialize_ehlo(domain)) {}

mail_from::mail_from(network::base_channel_t &channel,
                     const mime::mailbox_t &reverse_path)
    : command_t(channel, serialization::serialize_mail_from(reverse_path)) {}

rcpt_to::rcpt_to(network::base_channel_t &channel,
                 const mime::mailbox_t &forward_path)
    : command_t(channel, serialization::serialize_rcpt_to(forward_path)) {}

quit::quit(network::base_channel_t &channel)
    : command_t(channel, serialization::serialize_quit()) {}

starttls::starttls(network::base_channel_t &channel,
                   network::ssl_protocol_t ssl_protocol)
    : channel_(channel), data_(serialization::serialize_starttls()),
      ssl_protocol_(ssl_protocol) {}

server_reply starttls::execute(network::duration_t timeout) {
  channel_.write(data_, timeout);
  auto reply = receive_reply(channel_, timeout);

  if (reply.kind == server_reply::kind_t::positive_completion)
    channel_.upgrade_to_secure(ssl_protocol_, timeout);

  return reply;
}

} // namespace command
} // namespace s2smtp
