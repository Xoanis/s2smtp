// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.

#ifndef ADDRESS_SERIALIZATION_HPP
#define ADDRESS_SERIALIZATION_HPP

#include <sstream>
#include <string>

#include "s2smtp/mime/address.hpp"

namespace s2smtp {
namespace serialization {

void serialize(std::stringstream &s, const mime::mailbox_t &mailbox);
void serialize(std::stringstream &s, const mime::mailbox_list_t &mbl);
void serialize(std::stringstream &s, const mime::group_t &g);
void serialize(std::stringstream &s, const mime::address_t &addr);
void serialize(std::stringstream &s, const mime::address_list_t &adl);

} // namespace serialization
} // namespace s2smtp

#endif // ADDRESS_SERIALIZATION_HPP
