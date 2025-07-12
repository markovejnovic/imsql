#include "imsqlite/models/db.hpp"
#include "imsqlite/models/base_types.hpp"
#include "imsqlite/pch/std.hpp"

namespace imsql::models {

auto hash_value(const RelationshipEdge& edge) -> std::size_t {
  std::size_t seed = 0;
  boost::hash_combine(seed, edge.FromColumn);
  boost::hash_combine(seed, edge.ToColumn);
  return seed;
}

auto Db::Relationship(ColumnId column) const noexcept -> RelationshipType {
  const auto rels_r = columnInfo_.Relationships.equal_range(column);
  for (const auto& [_, edge] : std::ranges::subrange(rels_r.first, rels_r.second)) {
    if (edge.FromColumn == column) {
      return RelationshipType::LeftToRight;
    }
    if (edge.ToColumn == column) {
      return RelationshipType::RightToLeft;
    }
  }

  return RelationshipType::None;
}

} // namespace imsql::models
