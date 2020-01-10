// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.

#include "text_serialization.hpp"

#include <cppcodec/base64_rfc4648.hpp>

namespace s2smtp::serialization {

void serialize(std::stringstream &s, const mime::text_t &text) {
  if (text.data.empty())
    return;
  using base64 = cppcodec::base64_rfc4648;
  std::string result = "=?" + text.charset.to_string() + "?B?" +
                       base64::encode(text.data) + "?=";
  s << result;
}

} // namespace s2smtp::serialization
