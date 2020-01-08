// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.
//
// The module contains charset_t type representing text with arbitrary charset. 
// of message headers and inline body.

#ifndef CHARSET_HPP
#define CHARSET_HPP

#include <string>
#include <variant>
#include <vector>


#include "s2smtp/s2smtp_export.h"

namespace s2smtp {
namespace mime {

struct S2SMTP_EXPORT charset_t {
public:
  static charset_t default_charset;

  charset_t() : value_(default_charset.value_) {}
  charset_t(std::string txt) : value_(std::move(txt)) {}

  std::string to_string() const { return value_; }

  bool operator==(const charset_t &rh) const { return rh.value_ == value_; }

private:
  std::string value_;
};

namespace charsets {

const charset_t UTF_8 = charset_t("UTF-8");
const charset_t us_ascii = charset_t("us-ascii");
const charset_t iso_8859_1 = charset_t("ISO-8859-1");
const charset_t windows_1251 = charset_t("Windows-1251");
const charset_t windows_1252 = charset_t("Windows-1252");

} // namespace charsets

} // namespace mime
} // namespace s2smtp

#endif // CHARSET_HPP
