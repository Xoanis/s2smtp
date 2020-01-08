// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.
//
// The module contains text_t type to representing charset 
// of message headers and inline body.
// 
// References:
// https://tools.ietf.org/html/rfc2047
// https://tools.ietf.org/html/rfc2046

#ifndef TEXT_HPP
#define TEXT_HPP

#include <string>

#include "s2smtp/mime/charset.hpp"
#include "s2smtp/s2smtp_export.h"

namespace s2smtp {
namespace mime {

struct S2SMTP_EXPORT text_t {
  text_t() = default;
  text_t(std::string data, charset_t charset);
  text_t(std::string data);
  text_t(const char *data);
  text_t(const text_t &txt);
  text_t(text_t &&txt);
  text_t &operator=(std::string rh);
  text_t &operator=(text_t &&rh);
  text_t &operator=(const text_t &rh);
  text_t &operator=(const char *rh);
  bool operator==(const text_t &rh) const;
  bool operator!=(const text_t &rh) const;

  std::string data;
  charset_t charset;
};

} // namespace mime
} // namespace s2smtp

#endif // TEXT_HPP
