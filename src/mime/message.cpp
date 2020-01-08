// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.

#include "s2smtp/mime/message.hpp"

#define __STDC_WANT_LIB_EXT1__ 1
#if !defined(_XOPEN_SOURCE)
#define _XOPEN_SOURCE
#endif
#include <time.h>

namespace s2smtp {
namespace mime {

// https://en.cppreference.com/w/c/chrono/localtime
std::tm get_local_time() {
  std::time_t t = std::time(nullptr);
#ifdef __STDC_LIB_EXT1__
  std::tm timeinfo;
  localtime_s(&timeinfo, &t);
  return timeinfo;
#endif
  return *localtime(&t);
}

header_t::header_t(mailbox_t from, std::tm date)
    : from(std::move(from)), date(date) {}
header_t::header_t(mailbox_t from)
    : from(std::move(from)), date(get_local_time()) {}
bool text_part_t::empty() const { return content.data.empty(); }

text_part_t &text_part_t::operator=(const std::string &rh) {
  content = rh;
  return *this;
}

message_t::message_t(mailbox_t from, std::tm date)
    : header(std::move(from), date) {}
message_t::message_t(mailbox_t from) : header(std::move(from)) {}

} // namespace mime
} // namespace s2smtp