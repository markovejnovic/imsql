#ifndef IMSQL_MODELS_DB_MODEL_HPP
#define IMSQL_MODELS_DB_MODEL_HPP

#include "SQLiteCpp/Database.h"
#include "models/dg/nodes/values.hpp"
#include "pch.hpp"

namespace imsql {

class DbTableModel;
class DbModel;

class DbRelationshipEnforcement {
};

/// @brief Represents a column in a database table.
struct DbColumnModel {
  explicit DbColumnModel(DbTableModel* table, std::string name, const std::string& type,
                         bool nullable)
      : table_(table),
        name_(std::move(name)),
        type_(ValueTypeTagFromString(type)),
        nullable_(nullable) {}

  /// @brief Get the name of the column.
  [[nodiscard]] auto Name() const -> const std::string& { return name_; }

  /// @brief Get the type of the column.
  [[nodiscard]] auto Type() const -> dg::ValueTypeTag { return type_; }

  [[nodiscard]] auto Nullable() const noexcept -> bool {
    return nullable_;
  }

  /// @brief Get the rows of this column.
  /// @todo(marko) The return type is definitely a hack -- we're coupling dg::* with the
  ///              db_model.hpp. This should be separated out into a more generic.
  /// @todo(marko): This whole class should be templatized to support different value types. As-is,
  ///               it is naive that dg::Value is returned instead of whatever Type() is.
  [[nodiscard]] auto GetRows() const -> std::vector<dg::Value>;

  /// @todo(marko): This API is very fragile as it requires a primary key. There architectural
  ///               problems in my current design that I need to address before that can be
  ///               addressed.
  [[nodiscard]] auto GetValue(std::size_t primaryKey) const -> std::optional<dg::Value>;

private:
  std::string name_;
  DbTableModel* table_;
  dg::ValueTypeTag type_;
  bool nullable_ = false;

  static auto ValueTypeTagFromString(const std::string& type) -> dg::ValueTypeTag {
    // @todo(marko): This whole implementation is a hack. We should really be using SQLite's type
    // affinities to characterize the type of the column.
    if (type == "INTEGER" || type == "BOOLEAN") {
      return dg::ValueTypeTag::Int64;
    }
    if (type == "TEXT" || type == "VARCHAR" || type == "CHAR"
      || type == "CLOB" || type == "BLOB" || type == "JSON") {
      return dg::ValueTypeTag::String;
    }

    throw std::runtime_error(std::format("Unsupported column type: '{}'", type));
  }
};

/// @brief Represents a table in a database, containing multiple columns.
class DbTableModel {
public:
  explicit DbTableModel(DbModel* db, std::string name) : db_(db), name_(std::move(name)) {}

  /// @brief Get the name of the table.
  [[nodiscard]] auto Name() const -> const std::string& { return name_; }
  
  /// @brief Fetch the columns of the table.
  /// @{
  [[nodiscard]] auto Columns() const noexcept {
    return columns_
      | std::views::transform([](const auto& col) -> const DbColumnModel& { return *col; });
  }
  [[nodiscard]] auto Columns() noexcept -> auto& {
    return columns_;
  }
  /// @}

  [[nodiscard]] auto ColumnByName(std::string_view name) const -> const DbColumnModel&;
  [[nodiscard]] auto ColumnByName(std::string_view name) -> DbColumnModel&;

private:
  std::string name_;
  std::vector<std::unique_ptr<DbColumnModel>> columns_;
  DbModel* db_;

  friend class DbColumnModel;
};

class DbModel {
public:
  explicit DbModel(const std::string& path) : db_(path) {
    // @todo(marko) Loading local state here feels unnatural and wrong.
    LoadLocalState();
  }

  /// @brief Get the path to the database file.
  [[nodiscard]] auto Path() const -> const std::string& {
    return db_.getFilename();
  }

  /// @brief Add a table to the database.
  [[nodiscard]] auto Tables() const -> const auto& { return tables_; }
  [[nodiscard]] auto Tables() -> auto& { return tables_; }

  /// @brief Brief return an iterator of related columns in the database.
  /// @{
  [[nodiscard]] auto Relationships() const noexcept {
    return relationships_ | std::views::keys;
  }
  [[nodiscard]] auto Relationships() noexcept {
    return relationships_ | std::views::keys;
  }
  /// @}

  [[nodiscard]] auto TableByName(std::string_view name) const -> const DbTableModel&;
  [[nodiscard]] auto TableByName(std::string_view name) -> DbTableModel&;

private:
  SQLite::Database db_;

  // @todo(marko): Since we use raw pointers to refer between objects, we need to ensure that, when
  // the vector is resized, the pointers remain valid. This is annoying and potentially slow.
  std::vector<std::unique_ptr<DbTableModel>> tables_;

  boost::unordered_map<std::pair<DbColumnModel*, DbColumnModel*>, DbRelationshipEnforcement>
    relationships_;

  void LoadLocalState();

  friend class DbTableModel;
  friend class DbColumnModel;
};

} // namespace imsql

#endif // IMSQL_MODELS_DB_MODEL_HPP
