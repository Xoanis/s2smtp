// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.

#include "header_serialization.hpp"

#include <ctime>
#include <iomanip>

#include "address_serialization.hpp"
#include "overloaded.hpp"
#include "text_serialization.hpp"

namespace s2smtp::serialization {

void serialize(std::stringstream &s, const std::tm &datetime) {
  s << std::put_time(&datetime, "%a, %d %b %Y %H:%M:%S %z");
}

template <typename T>
void serialize_field(std::stringstream &s, const std::string &field_caption,
                     const T &v) {
  s << field_caption << ": ";
  serialize(s, v);
  s << "\r\n";
}

void serialize(std::stringstream &s, const mime::header_t &header) {
  s << "MIME-Version: 1.0\r\n";
  serialize_field(s, "From", header.from);
  if (!header.reply_to.empty())
    serialize_field(s, "Reply-To", header.reply_to);
  serialize_field(s, "To", header.to);
  if (!header.cc.empty())
    serialize_field(s, "Cc", header.cc);
  serialize_field(s, "Date", header.date);
  serialize_field(s, "Subject", header.subject);
}

} // namespace s2smtp::serialization
