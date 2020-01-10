// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.

#ifndef SMTP_PARSE_HPP
#define SMTP_PARSE_HPP

#include "s2smtp/smtp/server_reply.hpp"
#include <exception>
#include <string>

namespace s2smtp::parsing {

bool is_begin_of_multiline_reply(const std::string &data);
bool is_end_of_multiline_reply(const std::string &data);
server_reply parse_server_reply(const std::string &data);

} // namespace s2smtp::parsing

#endif // SMTP_PARSE_HPP
