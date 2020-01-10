// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.

#include "message_serialization.hpp"

#include <algorithm>

#include "serialization/mime/header_serialization.hpp"
#include <cppcodec/base64_rfc4648.hpp>

namespace s2smtp::serialization {

using base64 = cppcodec::base64_rfc4648;

std::string fixed_line_breaks(const std::string &base64_input) {
  constexpr const size_t len_lim =
      78; // https://tools.ietf.org/html/rfc5322 page 7

  size_t breaks_count =
      base64_input.empty() ? 0 : ((base64_input.size() - 1) / len_lim);

  if (breaks_count == 0)
    return base64_input;

  std::string result;
  result.reserve(base64_input.size() + 2 * breaks_count);

  auto it = base64_input.begin();
  while (breaks_count--) {
    auto it_next = std::next(it, len_lim);
    std::copy(it, it_next, std::back_inserter(result));
    result.append("\r\n");
    it = it_next;
  }

  std::copy(it, base64_input.end(), std::back_inserter(result));
  return result;
}

void serialize_inline_text_body(std::stringstream &s,
                                const mime::text_part_t &txt_part) {
  s << "Content-type: text/" << txt_part.text_content_type << "; charset=\""
    << txt_part.content.charset.to_string() << "\"\r\n";
  s << "Content-transfer-encoding: base64\r\n";
  s << "Content-Disposition: inline\r\n";
  s << "\r\n";
  s << fixed_line_breaks(base64::encode(txt_part.content.data));
}

void serialize(std::stringstream &s, const mime::attachment_t &attachment) {
  s << "Content-type: " << attachment.info().content_type << "\r\n";
  s << "Content-Disposition: attachment; filename=";
  s << attachment.info().name << ";\r\n";
  s << "Content-transfer-encoding: base64\r\n";
  s << "\r\n";
  s << fixed_line_breaks(base64::encode(attachment.content()));
}

std::string serialize(const mime::message_t &msg) {
  std::stringstream s;
  s.imbue(std::locale("C"));
  serialize(s, msg.header);

  if (msg.attachments.empty()) {
    if (msg.text.empty())
      return s.str();
    serialize_inline_text_body(s, msg.text);
  } else {
    std::string boundary = "------------------------------";
    s << "Content-Type: multipart/mixed; boundary=" << boundary << "\r\n";
    s << "\r\n--" << boundary << "\r\n";
    serialize_inline_text_body(s, msg.text);
    for (const auto &item : msg.attachments) {
      s << "\r\n--" << boundary << "\r\n";
      serialize(s, item);
    }
    s << "\r\n--" << boundary << "--";
  }
  return s.str();
}

} // namespace s2smtp::serialization
