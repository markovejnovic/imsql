#include "db_model.hpp"

namespace imsql {

namespace {

template <std::ranges::range Range>
auto FindByName(Range&& range, std::string_view name, std::string_view container) -> auto& {
  auto range_end = std::ranges::end(range);
  auto it = std::ranges::find_if(std::forward<Range>(range), [&](const auto& item) {
    return item.Name() == name;
  });

  if (it == range_end) {
    throw std::runtime_error(std::format("Item '{}' not found in '{}'", name, container));
  }

  return *it;
}

} // namespace

auto DbTableModel::ColumnByName(const std::string_view name) const -> const DbColumnModel& {
  return FindByName(columns_, name, Name());
}

auto DbTableModel::ColumnByName(const std::string_view name) -> DbColumnModel& {
  return FindByName(columns_, name, Name());
}

auto DbModel::TableByName(const std::string_view name) const -> const DbTableModel& {
  return FindByName(tables_, name, Path());
}

auto DbModel::TableByName(const std::string_view name) -> DbTableModel& {
  return FindByName(tables_, name, Path());
}

void DbModel::LoadLocalState() {
  const auto load_tables = [&]() {
    SQLite::Statement query(db_, "SELECT name FROM sqlite_master WHERE type='table';");
    while (query.executeStep()) {
      const auto tableName = query.getColumn(0).getString();
      DbTableModel tableModel{tableName};
      
      SQLite::Statement columnQuery(db_, "PRAGMA table_info('" + tableName + "');");
      while (columnQuery.executeStep()) {
        const auto columnName = columnQuery.getColumn(1).getString();
        const auto columnType = columnQuery.getColumn(2).getString();
        tableModel.Columns().emplace_back(columnName, columnType);
      }

      tables_.emplace_back(std::move(tableModel));
    }
  };

  const auto load_relationships = [&]() {
    for (auto& table : tables_) {
      SQLite::Statement query(db_, std::format("PRAGMA foreign_key_list({});", table.Name()));

      while (query.executeStep()) {
        auto* to_table = &table;
        const auto& to_table_str = table.Name();

        const auto from_table_str = query.getColumn(2).getString();
        // @note These are inverted relative to what SQLite outputs. SQLite outputs:
        // 0  | 1   | 2     | 3    | 4  | 5         | 6         |
        // id | seq | table | from | to | on_update | on_delete |
        // In their semantics, 'from' is the column of the current table, ie. the referent, not the
        // referenced column. We have inverted semantics in our application, so we need to flip
        // those
        const auto to_column_str = query.getColumn(3).getString();
        const auto from_column_str = query.getColumn(4).getString();

        auto* from_table = &TableByName(from_table_str);

        auto* from_column = &from_table->ColumnByName(from_column_str);
        auto* to_column = &to_table->ColumnByName(to_column_str);

        relationships_[std::make_pair(from_column, to_column)] = DbRelationshipEnforcement{};
      }
    }
  };

  load_tables();
  load_relationships();
}


} // namespace imsql
