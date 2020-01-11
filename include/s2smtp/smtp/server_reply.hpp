// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.
//
// The module contains type sender server_reply related to SMTP Replies.
// (rfc 5321, 4.2.  SMTP Replies [page 46])
//
// References:
// https://tools.ietf.org/html/rfc5321#section-4.2

#ifndef SERVER_REPLY_HPP
#define SERVER_REPLY_HPP

#include <cstdint>
#include <exception>
#include <string>
#include <vector>

#include "s2smtp/smtp/smtp_error.hpp"

namespace s2smtp {

struct server_reply {
  enum class kind_t {
    positive_completion = 2,
    positive_intermediate = 3,
    transient_negative_completion = 4,
    permanent_negative_completion = 5
  };

  enum class category_t {
    syntax = 0,
    information = 1,
    connections = 2,
    auth = 3,
    mail_system = 5
  };

  server_reply(uint16_t code, std::vector<std::string> &&text_lines);

  std::string text() const;

  const uint16_t code;
  const std::vector<std::string> text_lines;
  const kind_t kind;
  const category_t category;
};

class server_reply_error : public smtp_error {
public:
  explicit server_reply_error(server_reply reply);
};

} // namespace s2smtp

#endif // SERVER_REPLY_HPP
