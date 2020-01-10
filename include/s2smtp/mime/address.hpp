// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.
//
// The module contains types to compound a mime message
// related to rfc5322, 3.4.  Address Specification [page 16]

#ifndef ADDRESS_HPP
#define ADDRESS_HPP

#include <string>
#include <variant>
#include <vector>

#include "s2smtp/mime/text.hpp"
#include "s2smtp/s2smtp_export.h"

namespace s2smtp::mime {

struct S2SMTP_EXPORT mailbox_t {
  mailbox_t(std::string local_part, std::string domain, text_t display_name);
  mailbox_t(const std::string &addr_spec, text_t display_name);
  mailbox_t(const std::string &addr_spec);
  mailbox_t(const char *addr_spec);

  const std::string &local_part() const { return local_part_; }
  const std::string &domain() const { return domain_; }
  const text_t &display_name() const { return display_name_; }

  std::string to_string() const;

  bool operator==(const mailbox_t &rh) const;
  bool operator!=(const mailbox_t &rh) const;

private:
  void validate_not_empty() const;
  void validate(const std::string &str) const;

  std::string local_part_;
  std::string domain_;
  text_t display_name_;
};

using mailbox_list_t = std::vector<mailbox_t>;

struct S2SMTP_EXPORT group_t {
  text_t display_name;
  mailbox_list_t mailbox_list;
  std::string to_string() const;
};

using address_t = std::variant<mailbox_t, group_t>;
using address_list_t = std::vector<address_t>;

template <typename T> std::string to_string(const T &obj) {
  if constexpr (std::is_same_v<T, mailbox_list_t> ||
                std::is_same_v<T, address_list_t>) {
    std::string result;
    if (obj.empty())
      return result;
    auto it = obj.begin();
    result = it++->to_string();
    for (; it != obj.end(); ++it) {
      result += ", " + it->to_string();
    }
    return result;
  } else if constexpr (std::is_same_v<T, address_t>) {
    return std::visit([](auto v) { return v.to_string(); }, obj);
  } else {
    return obj.to_string();
  }
}

} // namespace s2smtp::mime

#endif // ADDRESS_HPP
