// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.

#include "address_serialization.hpp"
#include "text_serialization.hpp"

namespace s2smtp {
namespace serialization {

void serialize_addr_spec(std::stringstream &s, const mime::mailbox_t &mailbox) {
  s << mailbox.local_part() + '@' + mailbox.domain();
}

void serialize_angle_addr(std::stringstream &s,
                          const mime::mailbox_t &mailbox) {
  s << '<';
  serialize_addr_spec(s, mailbox);
  s << '>';
}

void serialize_name_addr(std::stringstream &s, const mime::mailbox_t &mailbox) {
  serialize(s, mailbox.display_name());
  s << ' ';
  serialize_angle_addr(s, mailbox);
}

void serialize(std::stringstream &s, const mime::mailbox_t &mailbox) {
  mailbox.display_name().data.empty() ? serialize_addr_spec(s, mailbox)
                                      : serialize_name_addr(s, mailbox);
}

void serialize(std::stringstream &s, const mime::mailbox_list_t &mbl) {
  if (mbl.empty())
    return;

  auto it = mbl.begin();
  auto end = mbl.end();

  serialize(s, *it++);

  for (; it != end; ++it) {
    s << ", ";
    serialize(s, *it);
  }
}

void serialize(std::stringstream &s, const mime::group_t &g) {
  serialize(s, g.display_name);
  s << ": ";
  if (!g.mailbox_list.empty()) {
    auto it = g.mailbox_list.begin();
    auto end = g.mailbox_list.end();
    serialize(s, *it++);
    for (; it != end; ++it) {
      s << ", ";
      serialize(s, *it);
    }
  }
  s << ';';
}

void serialize(std::stringstream &s, const mime::address_t &addr) {
  using mime::address_t;
  std::visit([&s](auto v) { serialize(s, v); }, addr);
}

void serialize(std::stringstream &s, const mime::address_list_t &adl) {
  if (adl.empty())
    return;
  auto it = adl.begin();
  auto end = adl.end();
  serialize(s, *it++);
  for (; it != end; ++it) {
    s << ", ";
    serialize(s, *it);
  }
}

} // namespace serialization
} // namespace s2smtp
