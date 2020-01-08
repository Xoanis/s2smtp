// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.

#ifndef COMMANDS_SERIALIZATION_HPP
#define COMMANDS_SERIALIZATION_HPP

#include "s2smtp/mime/address.hpp"
#include "s2smtp/mime/message.hpp"
#include <string>

namespace s2smtp {
namespace serialization {

std::string serialize_helo(const std::string &domain);
std::string serialize_ehlo(const std::string &domain);
std::string serialize_mail_from(const mime::mailbox_t &reverse_path);
std::string serialize_rcpt_to(const mime::mailbox_t &forward_path);
std::string serialize_data();
std::string serialize_data_content(const mime::message_t &msg);
std::string serialize_quit();
std::string serialize_starttls();

} // namespace serialization
} // namespace s2smtp

#endif // COMMANDS_SERIALIZATION_HPP
