// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.

#include "s2smtp/mime/address.hpp"
#include "s2smtp/mime/mime_error.hpp"

#include <algorithm>
#include <cctype>
#include <numeric>
#include <type_traits>

namespace s2smtp::mime {

mailbox_t::mailbox_t(std::string local_part, std::string domain,
                     text_t display_name)
    : local_part_(std::move(local_part)), domain_(std::move(domain)),
      display_name_(std::move(display_name)) {
  validate_not_empty();
  validate(local_part_);
  validate(domain_);
}

mailbox_t::mailbox_t(const std::string &addr_spec, text_t display_name)
    : display_name_(std::move(display_name)) {
  auto it = std::find(addr_spec.begin(), addr_spec.end(), '@');

  if (it == addr_spec.end())
    throw mime_error(
        std::string("Wrong addr_spec format (" + addr_spec + ")").c_str());

  local_part_ = std::string(addr_spec.begin(), it);
  domain_ = std::string(std::next(it), addr_spec.end());

  validate_not_empty();
  validate(local_part_);
  validate(domain_);
}

mailbox_t::mailbox_t(const std::string &addr_spec)
    : mailbox_t(addr_spec, std::string{}) {}

mailbox_t::mailbox_t(const char *addr_spec)
    : mailbox_t(std::string(addr_spec), std::string{}) {}

std::string mailbox_t::to_string() const {
  if (local_part_.empty() && domain_.empty())
    return "<expired variable>";
  auto addr_spec = local_part_ + '@' + domain_;
  return display_name_.data.empty()
             ? addr_spec
             : display_name_.data + " <" + addr_spec + '>';
}

bool mailbox_t::operator==(const mailbox_t &rh) const {
  return (rh.domain_ == domain_) && (rh.local_part_ == local_part_) &&
         (rh.display_name_ == display_name_);
}

bool mailbox_t::operator!=(const mailbox_t &rh) const { return !(rh == *this); }

void mailbox_t::validate_not_empty() const {
  if (local_part_.empty())
    throw mime_error("local part cannot be empty");
  if (domain_.empty())
    throw mime_error("domain cannot be empty");
}

void mailbox_t::validate(const std::string &str) const {
  static constexpr const char allowed_symbols[] = "!#$%&'*+-/=?^_`{|}~.";
  for (auto c : str) {
    if ((std::find(std::begin(allowed_symbols), std::end(allowed_symbols), c) ==
         std::end(allowed_symbols)) &&
        !std::isalnum(c)) {
      throw mime_error(std::string("Forbiden character in \"" + str + "\""));
    }
  }
}

std::string group_t::to_string() const {
  return display_name.data + ": " + mime::to_string(mailbox_list);
}

} // namespace s2smtp::mime
