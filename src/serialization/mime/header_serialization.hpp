// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.

#ifndef HEADER_SERIALIZATION_HPP
#define HEADER_SERIALIZATION_HPP

#include <sstream>
#include <string>

#include "s2smtp/mime/message.hpp"

namespace s2smtp {
namespace serialization {

void serialize(std::stringstream &s, const std::tm &datetime);
void serialize(std::stringstream &s, const mime::header_t &header);
}
} // namespace s2smtp

#endif // HEADER_SERIALIZATION_HPP
