#ifndef IMSQLITE_CONTROLLERS_DB_HPP
#define IMSQLITE_CONTROLLERS_DB_HPP

#include "imsqlite/models/db.hpp"
#include "imsqlite/models/persistent.hpp"
#include <SQLiteCpp/Database.h>
#include <filesystem>

namespace imsql::controllers {

class Db {
public:
  explicit Db(SQLite::Database&& database) noexcept;

  explicit Db(const std::filesystem::path& dbPath) noexcept;

  [[nodiscard]] auto PullFreshState() -> models::Db;
  void StorePersistentData(const models::Db& database);

private:
  void PullTableInfo(models::TableInfo& tableInfo, std::size_t& objectIdCounter);

  void PullColumnInfo(models::ColumnInfo& columnInfo,
                      models::TableInfo& tableInfo,
                      std::size_t& objectIdCounter);

  void PullRelationshipInfo(models::Db& database);

  void CreatePersistentTableIfNotExists();
  auto PullPersistentData() -> models::Persistent;
  [[nodiscard]] auto DefaultPersistentData() const -> models::Persistent;

  SQLite::Database db_;
};

} // namespace imsql::controllers

#endif // IMSQLITE_CONTROLLERS_DB_HPP
