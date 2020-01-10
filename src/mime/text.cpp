// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.

#include "s2smtp/mime/text.hpp"

namespace s2smtp::mime {

text_t::text_t(std::string data, charset_t charset)
    : data(std::move(data)), charset(charset) {}

text_t::text_t(std::string data) : data(std::move(data)) {}

text_t::text_t(const char *data) : data(data) {}

text_t::text_t(const text_t &txt) : data(txt.data), charset(txt.charset) {}

text_t::text_t(text_t &&txt)
    : data(std::move(txt.data)), charset(std::move(txt.charset)) {}

text_t &text_t::operator=(text_t &&rh) {
  data = std::move(rh.data);
  charset = std::move(rh.charset);
  return *this;
}

text_t &text_t::operator=(const text_t &rh) {
  data = rh.data;
  charset = rh.charset;
  return *this;
}

text_t &text_t::operator=(std::string rh) {
  data = std::move(rh);
  return *this;
}

text_t &text_t::operator=(const char *rh) {
  data = rh;
  return *this;
}

bool text_t::operator==(const text_t &rh) const {
  return (rh.data == data) && (rh.charset == charset);
}

bool text_t::operator!=(const text_t &rh) const { return !(*this == rh); }
} // namespace s2smtp::mime