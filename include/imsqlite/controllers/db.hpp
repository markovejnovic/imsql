#ifndef IMSQLITE_CONTROLLERS_DB_HPP
#define IMSQLITE_CONTROLLERS_DB_HPP

#include "imsqlite/controllers/identifier.hpp"
#include "imsqlite/models/db.hpp"
#include "imsqlite/models/persistent.hpp"
#include <SQLiteCpp/Database.h>
#include <filesystem>
#include <memory>

namespace imsql::controllers {

class Db {
public:
  explicit Db(
    SQLite::Database&& database,
    // TODO(marko): Can we avoid this shared_ptr?
    std::shared_ptr<Identifier> identCtrl
  ) noexcept;

  explicit Db(
    const std::filesystem::path& dbPath,
    // TODO(marko): Can we avoid this shared_ptr?
    std::shared_ptr<Identifier> identCtrl
  ) noexcept;

  void PullFreshState();
  void StorePersistentData();

  [[nodiscard]] constexpr auto CModel() const noexcept -> const models::Db& {
    return model_;
  }

  /// @todo(marko) Remove this function -- it's no better than a const_cast,
  /// really. Ultimately a better architecture would be to have a "persistent
  /// data controller".
  template <class F>
  void UpdatePersistentData(F&& mutator) noexcept {
    std::invoke(std::forward<F>(mutator), model_.persistentData_);
  }

private:
  void PullTableInfo(models::TableInfo& tableInfo, std::size_t& objectIdCounter);

  void PullColumnInfo(models::ColumnInfo& columnInfo,
                      models::TableInfo& tableInfo,
                      std::size_t& objectIdCounter);

  void PullRelationshipInfo(models::Db& database);

  void CreatePersistentTableIfNotExists();
  auto PullPersistentData() -> models::Persistent;
  [[nodiscard]] static auto DefaultPersistentData() -> models::Persistent;

  SQLite::Database db_;
  std::shared_ptr<Identifier> identCtrl_;
  models::Db model_;
};

} // namespace imsql::controllers

#endif // IMSQLITE_CONTROLLERS_DB_HPP
