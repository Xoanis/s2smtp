// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.

#ifndef TEXT_SERIALIZATION_HPP
#define TEXT_SERIALIZATION_HPP

#include <sstream>
#include <string>

#include "s2smtp/mime/text.hpp"

namespace s2smtp {
namespace serialization {

void serialize(std::stringstream &s, const mime::text_t &text);
}
} // namespace s2smtp

#endif // TEXT_SERIALIZATION_HPP
