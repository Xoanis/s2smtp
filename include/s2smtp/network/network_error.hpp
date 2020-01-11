// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.
//
// The module contains network_error class definition.
// network_error is related to different network errors.

#ifndef NETWORK_ERROR_HPP
#define NETWORK_ERROR_HPP

#include "s2smtp/s2smtp_error.hpp"
#include <system_error>

namespace s2smtp {

class network_error : public error {
public:
  explicit network_error(std::error_code e) : error(e.message()) {}
};

} // namespace s2smtp

#endif // NETWORK_ERROR_HPP