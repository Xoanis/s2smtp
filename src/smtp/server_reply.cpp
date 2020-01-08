// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.

#include "s2smtp/smtp/server_reply.hpp"

#include <boost/lexical_cast.hpp>
#include <numeric>

namespace s2smtp {

server_reply::server_reply(uint16_t code, std::vector<std::string> &&text_lines)
    : code(code), text_lines(std::move(text_lines)), kind([&code]() {
        auto res = code / 100;
        if (res < 2 || res > 5)
          throw smtp_error("Invalid reply code (" +
                           boost::lexical_cast<std::string>(code) + ')');
        return static_cast<kind_t>(res);
      }()),
      category([&code]() {
        auto res = (code / 10) % 10;
        if (!(res == 5 || res <= 3))
          throw smtp_error("Invalid reply code (" +
                           boost::lexical_cast<std::string>(code) + ')');
        return static_cast<category_t>(res);
      }()) {}

std::string server_reply::text() const {
  return text_lines.empty()
             ? std::string()
             : std::accumulate(std::next(text_lines.begin()), text_lines.end(),
                               text_lines.front(),
                               [](auto l, auto r) { return l + '\n' + r; });
}

server_reply_error::server_reply_error(server_reply reply)
    : smtp_error("Code: " + boost::lexical_cast<std::string>(reply.code) +
                 " message: \"" + reply.text() + "\"") {}

} // namespace s2smtp
