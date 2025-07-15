#ifndef IMSQL_MODELS_DB_MODEL_HPP
#define IMSQL_MODELS_DB_MODEL_HPP

#include "SQLiteCpp/Database.h"
#include "pch.hpp"

namespace imsql {

class DbTableModel;
class DbModel;

class DbRelationshipEnforcement {
};

/// @brief Represents a column in a database table.
struct DbColumnModel {
  explicit DbColumnModel(DbTableModel* table, std::string name, std::string type)
      : table_(table), name_(std::move(name)), type_(std::move(type)) {}

  /// @brief Get the name of the column.
  [[nodiscard]] auto Name() const -> const std::string& { return name_; }

  /// @brief Get the type of the column.
  [[nodiscard]] auto Type() const -> const std::string& { return type_; }

  /// @brief Get the rows of this column.
  template <class ValueType>
  [[nodiscard]] auto GetRows() const -> std::vector<ValueType>;

  template <class ValueType>
  [[nodiscard]] auto GetValue(std::size_t key) const -> ValueType;

private:
  std::string name_;
  std::string type_;
  DbTableModel* table_;
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

  template <class ValueType>
  [[nodiscard]] auto GetValue(std::string_view column_name, std::size_t key) const -> ValueType;

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
