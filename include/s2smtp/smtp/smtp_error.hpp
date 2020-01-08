// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.
//
// The module contains class smtp_error.
// smtp_error can arise during the "Commands/Replies" session.

#ifndef SMTP_ERROR_HPP
#define SMTP_ERROR_HPP

#include "s2smtp/s2smtp_error.hpp"

namespace s2smtp {
class smtp_error : public error {
public:
  smtp_error(std::string msg) : error(std::move(msg)) {}
};
} // namespace s2smtp

#endif // SMTP_ERROR_HPP
