#include "imsqlite/models/base_types.hpp"
#include "boost/functional/hash.hpp"
#include <boost/container_hash/hash.hpp>

namespace imsql::models {

auto hash_value(const ColumnId& columnId) -> std::size_t {
  return boost::hash_value(static_cast<std::size_t>(columnId));
}

auto hash_value(const TableId& columnId) -> std::size_t {
  return boost::hash_value(static_cast<std::size_t>(columnId));
}

} // namespace imsql::models
