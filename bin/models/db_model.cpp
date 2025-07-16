#include "db_model.hpp"
#include "models/dg/nodes/values.hpp"
#include <iterator>
#include <regex>
#include <string_view>
#include <utility>

namespace imsql {

namespace {

template <std::ranges::range Range>
auto FindByName(Range&& range, std::string_view name, std::string_view container) -> auto& {
  auto range_end = std::ranges::end(range);
  auto it = std::ranges::find_if(std::forward<Range>(range), [&](const auto& item) {
    return item->Name() == name;
  });

  if (it == range_end) {
    throw std::runtime_error(std::format("Item '{}' not found in '{}'", name, container));
  }

  return *it;
}

} // namespace

auto DbColumnModel::GetRows() const -> std::vector<dg::Value> {
  SQLite::Statement query(table_->db_->db_,
                          std::format("SELECT {} FROM {};", Name(), table_->Name()));

  std::vector<dg::Value> results;
  while (query.executeStep()) {
    const auto result = query.getColumn(0);
    if (result.isNull()) {
      // @todo(marko): I would like a better assert than BOOST_CONTRACT_ASSERT here, but the former
      // doesn't allow for error messages which are helpful.
      if (!Nullable()) {
        throw std::runtime_error(std::format(
          "Column '{}' in table '{}' is not nullable, but a NULL value was found.",
          Name(), table_->Name()));
      }
      results.emplace_back(dg::NullValue{});
      continue;
    }

    switch (Type()) {
      case dg::ValueTypeTag::Int64:
        results.emplace_back(dg::Int64Value(result));
        continue;
      case dg::ValueTypeTag::String:
        results.emplace_back(dg::StringValue(result.getString()));
        continue;
      // @todo(marko): This is quite hacky. Null shouldn't be treated as its own type here.
      case dg::ValueTypeTag::Null:
        std::unreachable();
        continue;
    }
  }

  return results;
}

auto DbColumnModel::GetValue(std::size_t primaryKey) const -> std::optional<dg::Value> {
  SQLite::Statement query(table_->db_->db_,
                          std::format("SELECT {} FROM {} WHERE id = {};",
                                      Name(), table_->Name(), primaryKey));

  if (query.executeStep()) {
    const auto result = query.getColumn(0);
    if (result.isNull()) {
      // @todo(marko): I would like a better assert than BOOST_CONTRACT_ASSERT here, but the former
      // doesn't allow for error messages which are helpful.
      if (!Nullable()) {
        throw std::runtime_error(std::format(
          "Column '{}' in table '{}' is not nullable, but a NULL value was found.",
          Name(), table_->Name()));
      }
      return dg::NullValue{};
    }

    switch (Type()) {
      case dg::ValueTypeTag::Int64:
        return dg::Int64Value(result);
      case dg::ValueTypeTag::String:
        return dg::StringValue(result.getString());
      case dg::ValueTypeTag::Null:
        std::unreachable();
    }
  }

  return std::nullopt;
}

auto DbTableModel::ColumnByName(const std::string_view name) const -> const DbColumnModel& {
  return *FindByName(columns_, name, Name());
}

auto DbTableModel::ColumnByName(const std::string_view name) -> DbColumnModel& {
  return *FindByName(columns_, name, Name());
}

auto DbModel::TableByName(const std::string_view name) const -> const DbTableModel& {
  return *FindByName(tables_, name, Path());
}

auto DbModel::TableByName(const std::string_view name) -> DbTableModel& {
  return *FindByName(tables_, name, Path());
}

void DbModel::LoadLocalState() {
  const auto load_tables = [&]() {
    SQLite::Statement query(db_, "SELECT name FROM sqlite_master WHERE type='table';");
    while (query.executeStep()) {
      const auto tableName = query.getColumn(0).getString();
      DbTableModel* tbl_model =
        tables_.emplace_back(std::make_unique<DbTableModel>(this, tableName)).get();
      
      SQLite::Statement columnQuery(db_, "PRAGMA table_info('" + tableName + "');");
      while (columnQuery.executeStep()) {
        const auto columnName = columnQuery.getColumn(1).getString();
        const auto columnType = columnQuery.getColumn(2).getString();
        const bool notNullable = columnQuery.getColumn(3).getInt() != 0;
        tbl_model->Columns().emplace_back(
          std::make_unique<DbColumnModel>(tbl_model, columnName, columnType, !notNullable));
      }

    }
  };

  const auto load_relationships = [&]() {
    for (auto& table : tables_) {
      SQLite::Statement query(db_, std::format("PRAGMA foreign_key_list({});", table->Name()));

      while (query.executeStep()) {
        auto* to_table = table.get();
        const auto& to_table_str = table->Name();

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
