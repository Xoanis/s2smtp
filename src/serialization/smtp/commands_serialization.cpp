// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.

#include "commands_serialization.hpp"
#include "serialization/mime/message_serialization.hpp"

namespace s2smtp::serialization {

std::string serialize_helo(const std::string &domain) {
  return "HELO " + domain + "\r\n";
}

std::string serialize_ehlo(const std::string &domain) {
  return "EHLO " + domain + "\r\n";
}

std::string serialize_path(const mime::mailbox_t &path) {
  return '<' + path.local_part() + '@' + path.domain() + '>';
}

std::string serialize_mail_from(const mime::mailbox_t &reverse_path) {
  return "MAIL FROM:" + serialize_path(reverse_path) + "\r\n";
}

std::string serialize_rcpt_to(const mime::mailbox_t &forward_path) {
  return "RCPT TO:" + serialize_path(forward_path) + "\r\n";
}

std::string serialize_data() { return "DATA\r\n"; }

std::string serialize_data_content(const mime::message_t &msg) {
  return serialize(msg) + "\r\n.\r\n";
}

std::string serialize_quit() { return "QUIT\r\n"; }

std::string serialize_starttls() { return "STARTTLS\r\n"; }

} // namespace s2smtp::serialization
