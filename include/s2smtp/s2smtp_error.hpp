// Copyright (c) 2020 Ivan Petrov <petroviv90@gmail.com>
// Licensed under the MIT license.
//
// The module contains a class error.
// All s2smtp related exceptions derive this class.

#ifndef S2SMTP_EXCEPTION_HPP
#define S2SMTP_EXCEPTION_HPP

#include <exception>
#include <string>

namespace s2smtp {

class error : public std::exception {
public:
  explicit error(std::string msg) : text_(std::move(msg)) {}
  const char *what() const noexcept override { return text_.c_str(); }

private:
  std::string text_;
};

} // namespace s2smtp

#endif