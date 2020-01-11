// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.
//
// The module contains types and functions to compound a mime
// message.
//
// type header_t is a simplify represention of message header fields
// (rfc 5322, 3.6.  Field Definitions [page 19])
// type text_part_t represents inline body part of mime message
// (rfc 2046, 4.1.  Text Media Type)
// (rfc 2183, 2.1  The Inline Disposition Type)
//
// References:
// https://tools.ietf.org/html/rfc5322
// https://tools.ietf.org/html/rfc2046
// https://tools.ietf.org/html/rfc2183

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <ctime>
#include <string>
#include <vector>

#include "s2smtp/mime/address.hpp"
#include "s2smtp/mime/attachment.hpp"
#include "s2smtp/mime/text.hpp"
#include "s2smtp/s2smtp_export.h"

namespace s2smtp::mime {

std::tm get_local_time();

struct header_t {
  header_t(mailbox_t from, std::tm date);
  explicit header_t(mailbox_t from);
  std::tm date;
  mailbox_t from;
  address_list_t reply_to;
  address_list_t to;
  address_list_t cc;
  address_list_t bcc;
  text_t subject;
};

using text_content_t = std::string;

namespace text_content_types {

const text_content_t plain = "plain";
const text_content_t html = "html";

} // namespace text_content_types

struct text_part_t {
  text_content_t text_content_type = text_content_types::plain;
  text_t content;
  bool empty() const;
  text_part_t &operator=(const std::string &);
};

struct message_t {
  message_t(mailbox_t from, std::tm date);
  explicit message_t(mailbox_t from);
  header_t header;
  text_part_t text;
  attachment_list_t attachments;
};

} // namespace s2smtp::mime

#endif // MESSAGE_HPP
