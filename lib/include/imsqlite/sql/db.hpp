#ifndef IMSQLITE_SQL_DB_HPP
#define IMSQLITE_SQL_DB_HPP
#include "SQLiteCpp/Statement.h"
#include <format>
#include <memory>
#include <ranges>
#pragma once

#include "SQLiteCpp/Database.h"
#include <filesystem>
#include "table.hpp"

namespace imsql::sql {

// TODO(marko): This view is kind of over-engineered... I'm afraid we're going
// to suffer performance because of this engineering...
class DbTablesView final : public std::ranges::view_interface<DbTablesView> {
private:
  struct iterator {
    using value_type = Table;
    using difference_type = std::ptrdiff_t;
    using iterator_concept = std::input_iterator_tag;
    using reference = value_type;
    using pointer = void;

    friend class DbTablesView;

    auto operator*() const -> Table;
    auto operator++() -> iterator&;
    void operator++(int) { ++(*this); }
    auto operator==(std::default_sentinel_t /*end*/) const -> bool;

  private:
    SQLite::Statement query_;
    bool haveData_ = true;

    explicit iterator(SQLite::Statement query)
        : query_(std::move(query)), haveData_(query_.executeStep()) {}
  };

public:
  explicit constexpr DbTablesView(const SQLite::Database& db) : db_(db) {}

  [[nodiscard]] auto begin() const -> iterator {
    // Create an iterator with the query to fetch table names.
    return iterator{SQLite::Statement{
      db_, "SELECT name FROM sqlite_master WHERE type = 'table';"
    }};
  }
  [[nodiscard]] auto end() const -> std::default_sentinel_t {
    return std::default_sentinel;
  }

private:
  const SQLite::Database& db_;
};

class DbTableColumnsView
    : public std::ranges::view_interface<DbTableColumnsView> {
private:
  struct iterator {
    using value_type = Table::Column;
    using difference_type = std::ptrdiff_t;
    using iterator_concept = std::input_iterator_tag;
    using reference = value_type;
    using pointer = void;

    friend class DbTableColumnsView;

    auto operator*() const -> value_type {
      return Table::Column{
        query_.getColumn(1).getString(), // Column name
        query_.getColumn(2).getString()  // Column type
      };
    }
    auto operator++() -> iterator& {
      if (haveData_) {
        haveData_ = query_.executeStep();
      }
      return *this;
    }
    void operator++(int) { ++(*this); }
    auto operator==(std::default_sentinel_t /*end*/) const -> bool {
      return !haveData_;
    }

  private:
    SQLite::Statement query_;
    bool haveData_ = true;

    explicit iterator(SQLite::Statement query)
        : query_(std::move(query)), haveData_(query_.executeStep()) {}
  };

public:
  explicit constexpr DbTableColumnsView(
    const SQLite::Database& db,
    const std::string& tableName
  ) : db_(db), tableName_(tableName) {}

  [[nodiscard]] auto begin() const -> iterator {
    // Create an iterator with the query to fetch table names.
    return iterator{SQLite::Statement{
      db_,
      // TODO(marko): SQL Injection against who?
      std::format("PRAGMA table_info('{}');", tableName_)
    }};
  }
  [[nodiscard]] auto end() const -> std::default_sentinel_t {
    return std::default_sentinel;
  }

private:
  const SQLite::Database& db_;
  const std::string& tableName_;
};

class Db {
public:
  explicit Db(SQLite::Database database) : db_(std::move(database)) {}

  static auto FromFile(const std::filesystem::path& path) -> Db;

  [[nodiscard]] constexpr auto Tables() const -> DbTablesView {
    return DbTablesView{db_};
  }


  [[nodiscard]] constexpr auto TableColumns(
    const std::string& tableName
  ) const -> DbTableColumnsView {
    return DbTableColumnsView{db_, tableName};
  }

private:
  // TODO(marko): pimpl?
  SQLite::Database db_;
};

} // namespace imsql::sql

namespace std::ranges {

template <>
inline constexpr bool enable_borrowed_range<imsql::sql::DbTablesView> = true;

} // namespace std::ranges

#endif // IMSQLITE_SQL_DB_HPP
