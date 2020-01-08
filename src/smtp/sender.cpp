// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.

#include "s2smtp/smtp/sender.hpp"

#include "overloaded.hpp"
#include "smtp/commands.hpp"

namespace s2smtp {

sender_t::sender_t(network::base_channel_t &channel, std::string domain,
                   bool starttls, network::ssl_protocol_t ssl_protocol,
                   opt_credentials credentials)
    : channel_(channel), domain_(domain), starttls_(starttls),
      ssl_protocol_(ssl_protocol), credentials_(credentials) {}

void sender_t::submit(const mime::message_t &msg,
                      network::duration_t command_timeout) {
  // rfc 5321, Sequencing of Commands and Replies
  // (https://tools.ietf.org/html/rfc5321#section-4.3)

  check_(command::receive_greeting(channel_, command_timeout), command_timeout);

  auto try_ = [&](auto &&command) {
    return check_(command.execute(command_timeout), command_timeout);
  };

  auto ehlo_reply = command::ehlo(channel_, domain_).execute(command_timeout);

  if (ehlo_reply.kind == server_reply::kind_t::permanent_negative_completion &&
      ehlo_reply.category == server_reply::category_t::syntax) {
    // Assumed this is an old server that doesn't support esmtp.
    // We issue old-style 'HELO' command if all is ok then discard
    // 'starttls' options and credentials
    try_(command::helo(channel_, domain_));
    starttls_ = false;
    credentials_ = std::nullopt;
  }

  if (starttls_) {
    try_(command::starttls(channel_, ssl_protocol_));
    try_(command::ehlo(channel_, domain_));
  }

  if (credentials_) {
    try_(command::auth_login(channel_, credentials_->user,
                             credentials_->password));
  }

  try_(command::mail_from(channel_, msg.header.from));
  rcpt_to_for_address_list(msg.header.to, command_timeout);
  rcpt_to_for_address_list(msg.header.cc, command_timeout);
  rcpt_to_for_address_list(msg.header.bcc, command_timeout);

  try_(command::data(channel_, msg));

  command::quit(channel_).execute(command_timeout);
}

void sender_t::rcpt_to_for_address_list(const mime::address_list_t &addr_list,
                                        network::duration_t timeout) {
  for (const auto &addr : addr_list) {
    std::visit(
        overloaded{
            [&](const mime::mailbox_t &m) {
              check_(command::rcpt_to(channel_, m).execute(timeout), timeout);
            },
            [&](const mime::group_t &g) {
              for (const auto &m : g.mailbox_list)
                check_(command::rcpt_to(channel_, m).execute(timeout), timeout);
            }},
        addr);
  }
}

// we have to ensure that the server reply is positive,
// otherwise we gentle send 'quit' command and throw exception
// cause we haven't strategy yet to handle negatives replies.
void sender_t::check_(const server_reply &reply, network::duration_t timeout) {
  if (reply.kind != server_reply::kind_t::positive_completion) {
    command::quit(channel_).execute(timeout);
    throw server_reply_error(reply);
  }
}

} // namespace s2smtp
