// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.
//
// The module contains mime_error class definition.
// mime_error is related to message compounding errors.

#ifndef MIME_EXCEPTION_HPP
#define MIME_EXCEPTION_HPP

#include "s2smtp/s2smtp_error.hpp"

namespace s2smtp {

class mime_error : public error {
public:
  mime_error(std::string msg) : error(std::move(msg)) {}
};

} // namespace s2smtp

#endif // MIME_EXCEPTION_HPP
