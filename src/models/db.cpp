#include "imsqlite/models/db.hpp"
#include <boost/functional/hash.hpp>
#include <ranges>

namespace imsql::models {

auto hash_value(const RelationshipEdge& edge) -> std::size_t {
  std::size_t seed = 0;
  boost::hash_combine(seed, edge.FromColumn);
  boost::hash_combine(seed, edge.ToColumn);
  return seed;
}

} // namespace imsql::models
