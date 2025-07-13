#ifndef IMSQL_MODELS_DB_MODEL_HPP
#define IMSQL_MODELS_DB_MODEL_HPP

#include "SQLiteCpp/Database.h"
#include "pch.hpp"

namespace imsql {

class DbRelationshipEnforcement {
};

/// @brief Represents a column in a database table.
struct DbColumnModel {
  explicit DbColumnModel(std::string name, std::string type)
      : name_(std::move(name)), type_(std::move(type)) {}

  /// @brief Get the name of the column.
  [[nodiscard]] auto Name() const -> const std::string& { return name_; }

  /// @brief Get the type of the column.
  [[nodiscard]] auto Type() const -> const std::string& { return type_; }

private:
  std::string name_;
  std::string type_;

};

/// @brief Represents a table in a database, containing multiple columns.
class DbTableModel {
public:
  explicit DbTableModel(std::string name) : name_(std::move(name)) {}

  /// @brief Get the name of the table.
  [[nodiscard]] auto Name() const -> const std::string& { return name_; }
  
  /// @brief Fetch the columns of the table.
  /// @{
  [[nodiscard]] auto Columns() const noexcept -> const std::vector<DbColumnModel>& {
    return columns_;
  }
  [[nodiscard]] auto Columns() noexcept -> std::vector<DbColumnModel>& {
    return columns_;
  }
  /// @}

  [[nodiscard]] auto ColumnByName(std::string_view name) const -> const DbColumnModel&;
  [[nodiscard]] auto ColumnByName(std::string_view name) -> DbColumnModel&;

private:
  std::string name_;
  std::vector<DbColumnModel> columns_;
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
  [[nodiscard]] auto Tables() const -> const std::vector<DbTableModel>& { return tables_; }
  [[nodiscard]] auto Tables() -> std::vector<DbTableModel>& { return tables_; }

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

  std::vector<DbTableModel> tables_;

  boost::unordered_map<std::pair<DbColumnModel*, DbColumnModel*>, DbRelationshipEnforcement>
    relationships_;

  void LoadLocalState();
};

} // namespace imsql

#endif // IMSQL_MODELS_DB_MODEL_HPP
