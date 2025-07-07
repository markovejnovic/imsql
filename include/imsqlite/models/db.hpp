#ifndef IMSQLITE_MODELS_DB_HPP
#define IMSQLITE_MODELS_DB_HPP

#include <boost/bimap.hpp>
#include <unordered_map>
#include "base_types.hpp"

namespace imsql::models {

struct Db {
private:
  struct TableInfo {
    boost::bimaps::bimap<TableId, std::string> Names;
    std::unordered_multimap<TableId, ColumnId> ColumnMemberships;
  };

public:
  [[nodiscard]] auto Path() const noexcept -> std::string {
    return path_;
  }

  [[nodiscard]] auto TableNames() const noexcept {
    return tableInfo_.Names.right;
  }

private:
  std::string path_;

  TableInfo tableInfo_;

  std::unordered_map<RelationshipEdge, Relationship, boost::hash<RelationshipEdge>> relationships_;

  std::unordered_map<ColumnId, Column> columns_;

  friend class DbController;
};

} // namespace imsql::models

#endif // IMSQLITE_MODELS_DB_HPP
