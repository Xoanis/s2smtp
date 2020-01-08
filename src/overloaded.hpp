#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <type_traits>

template <class T> struct always_false : std::false_type {};
template <class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template <class... Ts> overloaded(Ts...)->overloaded<Ts...>;

#endif // UTILITIES_HPP
