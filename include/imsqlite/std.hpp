#ifndef IMSQLITE_STD_HPP
#define IMSQLITE_STD_HPP

#include <boost/bimap.hpp>
#include <boost/functional/hash.hpp>
#include <boost/none.hpp>
#include <boost/optional.hpp>
#include <boost/serialization/version.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <cmath>
#include <compare>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <filesystem>
#include <format>
#include <map>
#include <memory>
#include <ranges>
#include <string>

namespace imsql {

template <class T>
auto strof(const T& value) -> std::string {
  using std::to_string;
  return to_string(value);
}

} // namespace imsql

#endif // IMSQLITE_STD_HPP
