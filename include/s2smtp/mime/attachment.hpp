// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.
//
// The module contains types to compound a mime message.
// Type attachment_t represents message body part with the attachment disposition type.
// References:
// https://tools.ietf.org/html/rfc2183
// https://tools.ietf.org/html/rfc2045
// https://tools.ietf.org/html/rfc2046

#ifndef ATTACHMENT_HPP
#define ATTACHMENT_HPP

#include <istream>
#include <string>
#include <vector>

#include "s2smtp/mime/mime_error.hpp"
#include "s2smtp/s2smtp_export.h"

namespace s2smtp::mime {

struct S2SMTP_EXPORT attachment_info_t {
  attachment_info_t(std::string name, std::string content_type);
  explicit attachment_info_t(std::string name);
  bool operator==(const attachment_info_t &rh) const;
  std::string name;
  std::string content_type;
  static const std::string default_content_type;
};

struct S2SMTP_EXPORT attachment_t {
  using octetvector_t = std::vector<char>;

  attachment_t(attachment_info_t info, std::istream &stream);
  attachment_t(attachment_info_t info, std::istream &&stream);
  attachment_t(const std::string &attach_name, std::istream &stream);
  attachment_t(const std::string &attach_name, std::istream &&stream);
  explicit attachment_t(const std::string &file_path);

  const attachment_info_t &info() const { return info_; }
  const octetvector_t &content() const { return content_; }

private:
  attachment_info_t info_;
  octetvector_t content_;
};

using attachment_list_t = std::vector<attachment_t>;

} // namespace s2smtp::mime
#endif // ATTACHMENT_HPP
