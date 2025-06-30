#ifndef IMSQLITE_EX_STR_HPP
#define IMSQLITE_EX_STR_HPP
#pragma once

namespace imsql::ex::str {

template <class T, class K>
[[nodiscard]] constexpr auto Contains(
  const T& haystack,
  const K& needle
) -> bool {
  return haystack.find(needle) != T::npos;
}

} // namespace imsql::ex::str

#endif // IMSQLITE_EX_STR_HPP
