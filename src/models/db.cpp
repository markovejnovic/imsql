#include <boost/bimap.hpp>
#include <unordered_map>

export module imsql.models.db;

export namespace imsql::models {

export struct Db {
public:
  [[nodiscard]] auto Path() const noexcept -> std::string {
    return path_;
  }

  [[nodiscard]] auto TableNames() const noexcept {
    return tableNames_.right;
  }

  [[nodiscard]] auto Tables() const noexcept {
    return tables_;
  }

  [[nodiscard]] auto ColumnInfo(ColumnId columnId) const -> Column {
    return columns_.at(columnId);
  }

  [[nodiscard]] auto CColumns() const noexcept {
    return columns_;
  }

  [[nodiscard]] auto Relationships() const noexcept {
    return relationships_;
  }
  
private:
  std::string path_;

  boost::bimaps::bimap<TableId, std::string> tableNames_;
  std::unordered_map<TableId, Table> tables_;
  std::unordered_map<RelationshipEdge, Relationship, boost::hash<RelationshipEdge>> relationships_;

  boost::bimaps::bimap<ColumnId, TableId> columnMemberships_;
  std::unordered_map<ColumnId, Column> columns_;

  friend class DbController;
};

} // namespace imsql::models
