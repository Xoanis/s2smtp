// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.

#ifndef MIME_MESSAGE_SERIALIZATION_HPP
#define MIME_MESSAGE_SERIALIZATION_HPP

#include <sstream>
#include <string>

#include "s2smtp/mime/attachment.hpp"
#include "s2smtp/mime/message.hpp"

namespace s2smtp {
namespace serialization {

std::string serialize(const mime::message_t &msg);
}
} // namespace s2smtp

#endif // MIME_MESSAGE_SERIALIZATION_HPP
