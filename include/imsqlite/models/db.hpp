#ifndef IMSQLITE_MODELS_DB_HPP
#define IMSQLITE_MODELS_DB_HPP

#include <boost/bimap.hpp>
#include <boost/functional/hash.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <map>
#include <ranges>
#include "base_types.hpp"
#include "persistent.hpp"

namespace imsql::controllers {
class Db;
} // namespace imsql::controllers

namespace imsql::models {

struct RelationshipEdge {
  ColumnId FromColumn;
  ColumnId ToColumn;

  [[nodiscard]] constexpr auto operator==(const RelationshipEdge& other) const noexcept -> bool {
    return (FromColumn == other.FromColumn && ToColumn == other.ToColumn) ||
           (FromColumn == other.ToColumn && ToColumn == other.FromColumn);
  }

  [[nodiscard]] constexpr auto operator!=(const RelationshipEdge& other) const noexcept -> bool {
    return !(*this == other);
  }
};

struct RelationshipEnforcement {
  OnDeleteOnUpdateAction OnUpdateAction;
  OnDeleteOnUpdateAction OnDeleteAction;
};

struct ColumnInfo {
  boost::bimaps::bimap<ColumnId, std::pair<TableId, std::string>> Names;
  boost::unordered_map<ColumnId, std::string> Types;
  boost::unordered_set<ColumnId> NullColumns;
  boost::unordered_set<ColumnId> PkColumns;
  boost::unordered_map<ColumnId, RelationshipEdge> Relationships;
};

struct TableInfo {
  boost::bimaps::bimap<TableId, std::string> Names;
  std::multimap<TableId, ColumnId> ColumnMemberships;
};

struct RelationshipInfo {
  boost::unordered_map<RelationshipEdge, RelationshipEnforcement> Relationships;
};

auto hash_value(const RelationshipEdge& edge) -> std::size_t;

struct Db {
public:
  enum class RelationshipType : std::uint8_t {
    None,
    LeftToRight,
    RightToLeft,
  };

  [[nodiscard]] auto Path() const noexcept -> std::string {
    return path_;
  }

  [[nodiscard]] auto TableNames() const noexcept {
    return tableInfo_.Names.right;
  }

  [[nodiscard]] auto Tables() const noexcept {
    return tableInfo_.Names.left;
  }

  [[nodiscard]] auto Columns(const TableId& table) const {
    const auto [begin, end] = tableInfo_.ColumnMemberships.equal_range(table);
    return std::ranges::subrange(begin, end)
      | std::views::transform([](const auto& pair) { return pair.second; });
  }

  [[nodiscard]] constexpr auto TableName(TableId table) const -> std::string {
    return tableInfo_.Names.left.at(table);
  }

  [[nodiscard]] auto TableByName(const std::string& tableName) const {
    return tableInfo_.Names.right.at(tableName);
  }

  [[nodiscard]] constexpr auto ColumnName(ColumnId column) const -> std::string {
    return columnInfo_.Names.left.at(column).second;
  }

  [[nodiscard]] constexpr auto ColumnByName(TableId tableId,
                                            const std::string& column) const -> ColumnId {
    return columnInfo_.Names.right.at(std::make_pair(tableId, column));
  }

  [[nodiscard]] auto ColumnTypeString(ColumnId column) const -> std::string {
    return columnInfo_.Types.at(column);
  }

  [[nodiscard]] constexpr auto ColumnIsNull(ColumnId column) const noexcept -> bool {
    return columnInfo_.NullColumns.contains(column);
  }

  [[nodiscard]] constexpr auto ColumnIsPk(ColumnId column) const noexcept -> bool {
    return columnInfo_.PkColumns.contains(column);
  }

  [[nodiscard]] constexpr auto PersistentData() noexcept -> Persistent& {
    return persistentData_;
  }

  [[nodiscard]] constexpr auto Relationship(
    ColumnId lhs,
    ColumnId rhs
  ) const noexcept -> RelationshipType {
    const RelationshipEdge ltr = {.FromColumn = lhs, .ToColumn = rhs};
    const RelationshipEdge rtl = {.FromColumn = rhs, .ToColumn = lhs};

    if (relationshipInfo_.Relationships.contains(ltr)) {
      return RelationshipType::LeftToRight;
    }
    if (relationshipInfo_.Relationships.contains(rtl)) {
      return RelationshipType::RightToLeft;
    }

    return RelationshipType::None;
  }

  [[nodiscard]] constexpr auto HasRelationships(
    ColumnId column
  ) const noexcept -> bool {
    return columnInfo_.Relationships.contains(column);
  }

private:
  std::string path_;

  TableInfo tableInfo_;
  ColumnInfo columnInfo_;
  RelationshipInfo relationshipInfo_;
  Persistent persistentData_;

  friend class controllers::Db;
};

} // namespace imsql::models

#endif // IMSQLITE_MODELS_DB_HPP
