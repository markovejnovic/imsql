#include "imsqlite/controllers/db.hpp"
#include "imsqlite/controllers/identifier.hpp"
#include "imsqlite/models/base_types.hpp"
#include "imsqlite/models/db.hpp"
#include "imsqlite/models/persistent.hpp"
#include "imsqlite/serde/serde.hpp"
#include "sqlite3.h"
#include <boost/contract/public_function.hpp>
#include <cstdio>
#include <format>
#include <sstream>
#include <utility>

namespace imsql::controllers {

Db::Db(SQLite::Database&& database, std::shared_ptr<Identifier> identCtrl) noexcept
    : db_(std::move(database)), identCtrl_(std::move(identCtrl)) {}

Db::Db(const std::filesystem::path& dbPath, std::shared_ptr<Identifier> identCtrl) noexcept
    : db_(SQLite::Database{dbPath.string(), SQLite::OPEN_READWRITE}),
      identCtrl_(std::move(identCtrl)) {}

void Db::PullTableInfo(models::TableInfo& tableInfo, std::size_t& objectIdCounter) {
  SQLite::Statement tbl_name_query(db_, "SELECT name FROM sqlite_master WHERE type='table';");
  while (tbl_name_query.executeStep()) {
    const auto table_id = models::TableId(objectIdCounter++);
    const auto table_name = tbl_name_query.getColumn(0).getString();

     tableInfo.Names.insert({table_id, table_name});
  }
}

void Db::PullColumnInfo(models::ColumnInfo& columnInfo,
                        models::TableInfo& tableInfo,
                        std::size_t& objectIdCounter) {

  const auto pull_for_table = [&] (models::TableId tableId) {
    const auto table_name = tableInfo.Names.left.at(tableId);
    SQLite::Statement query(db_, std::format("PRAGMA table_info({});", table_name));

    while (query.executeStep()) {
      const auto column_id = models::ColumnId(objectIdCounter++);

      const auto column_name = query.getColumn(1).getString();
      const auto column_type = query.getColumn(2).getString();
      const bool not_null = query.getColumn(3).getInt() != 0;
      // TODO(marko): Handle default value.
      const bool is_primary_key = query.getColumn(5).getInt() != 0;

      // Populate the column info.
      columnInfo.Names.insert({column_id, std::make_pair(tableId, column_name)});
      columnInfo.Types.insert({column_id, column_type});
      if (not_null) {
        columnInfo.NullColumns.insert(column_id);
      }
      if (is_primary_key) {
        columnInfo.PkColumns.insert(column_id);
      }

      // Super important we don't forget to actually back-link to the table.
      tableInfo.ColumnMemberships.insert({tableId, column_id});
    }
  };

  for (const auto& [table_id, _] : tableInfo.Names.left) {
    pull_for_table(table_id);
  }
}

void Db::PullRelationshipInfo(models::Db& database) {
  const auto pull_for_table = [&] (models::TableId tableId) {
    const auto table_name = database.tableInfo_.Names.left.at(tableId);
    SQLite::Statement query(db_, std::format("PRAGMA foreign_key_list({});", table_name));

    while (query.executeStep()) {
      const auto to_table_name = query.getColumn(2).getString();
      const auto column_name_from = query.getColumn(3).getString();
      const auto column_name_to = query.getColumn(4).getString();
      const auto update_action =
        models::OnDeleteOnUpdateActionFromString(query.getColumn(5).getString());
      const auto del_action =
        models::OnDeleteOnUpdateActionFromString(query.getColumn(6).getString());

      const models::ColumnId column_from = database.ColumnByName(tableId, column_name_from);
      const models::ColumnId column_to = database.ColumnByName(database.TableByName(to_table_name),
                                                               column_name_to);

      const models::RelationshipEdge edge{
        .FromColumn = column_from,
        .ToColumn = column_to,
      };

      database.columnInfo_.Relationships.emplace(column_from, edge);
      database.columnInfo_.Relationships.emplace(column_to, edge);

      database.relationshipInfo_.Relationships.emplace(
        edge,
        models::RelationshipEnforcement{
          .OnUpdateAction = update_action,
          .OnDeleteAction = del_action,
        }
      );
    }
  };

  for (const auto& [table_id, _] : database.tableInfo_.Names.left) {
    pull_for_table(table_id);
  }
}

void Db::CreatePersistentTableIfNotExists() {
  db_.exec("CREATE TABLE IF NOT EXISTS __imsql_data ("
           "id INTEGER PRIMARY KEY, "
           "data TEXT NOT NULL);");
}

auto Db::DefaultPersistentData() -> models::Persistent {
  // Return a default persistent data object.
  return models::Persistent{
    .TableNodePositions = boost::unordered_map<models::TableId, ui::Vec2>{}
  };
}

auto Db::PullPersistentData() -> models::Persistent {
  // Fetch the persistent data from the database, if available.
  try {
    SQLite::Statement query(db_, "SELECT data FROM __imsql_data WHERE id = 1;");
    if (query.executeStep()) {
      const auto data = query.getColumn(0).getString();
      std::istringstream iss(data);
      return serde::Deserialize<models::Persistent>(iss);
    } 

    return DefaultPersistentData();
  } catch (const SQLite::Exception& e) {
    if (e.getErrorCode() != SQLITE_ERROR) {
      throw;
    }

    return DefaultPersistentData();
  }
}

void Db::PullFreshState() {
  model_.path_ = db_.getFilename();

  // TODO(marko): The object identification is quite fragile.
  std::size_t object_id = 0;

  PullTableInfo(model_.tableInfo_, object_id);
  PullColumnInfo(model_.columnInfo_, model_.tableInfo_, object_id);
  PullRelationshipInfo(model_);
  model_.persistentData_ = PullPersistentData();
}

void Db::StorePersistentData() {
  CreatePersistentTableIfNotExists();
  std::ostringstream oss;
  serde::Serialize(oss, model_.persistentData_);

  SQLite::Statement query(db_, "INSERT OR REPLACE INTO __imsql_data (id, data) VALUES (1, ?);");
  query.bind(1, oss.str());
  query.exec();
}

} // namespace imsql::controllers
