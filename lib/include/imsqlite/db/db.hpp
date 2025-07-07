#ifndef IMSQLITE_DB_DB_HPP
#define IMSQLITE_DB_DB_HPP

#include "SQLiteCpp/Database.h"
#include "SQLiteCpp/Exception.h"
#include "SQLiteCpp/Statement.h"
#include "imgui.h"
#include <algorithm>
#include <cstdio>
#include <filesystem>
#include <format>
#include <optional>
#include <string_view>
#include <unordered_map>
#include <atomic>
#include <vector>
#include <ranges>
#include <algorithm>
#include <boost/bimap.hpp>
#include <boost/contract.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/unordered_map.hpp>

using TableId = std::size_t;
using ColumnId = std::size_t;

namespace boost::serialization {

template <class Archive>
void serialize(Archive& archive, ImVec2& vec,
              const unsigned int /*version*/) {
  archive & vec.x;
  archive & vec.y;
}

} // namespace boost::serialization

enum class OnDeleteOnUpdateAction : uint8_t {
  NoAction,
  Restrict,
  SetNull,
  SetDefault,
  Cascade,
};

auto OnDeleteOnUpdateActionFromString(const std::string& action) -> OnDeleteOnUpdateAction {
  if (action == "NO ACTION") return OnDeleteOnUpdateAction::NoAction;
  if (action == "RESTRICT") return OnDeleteOnUpdateAction::Restrict;
  if (action == "SET NULL") return OnDeleteOnUpdateAction::SetNull;
  if (action == "SET DEFAULT") return OnDeleteOnUpdateAction::SetDefault;
  if (action == "CASCADE") return OnDeleteOnUpdateAction::Cascade;

  throw std::invalid_argument("Unknown action: " + action);
}

struct RelationshipEdge {
  ColumnId FromColumn;
  ColumnId ToColumn;

  [[nodiscard]] constexpr auto operator==(
    const RelationshipEdge& other
  ) const noexcept -> bool {
    return FromColumn == other.FromColumn && ToColumn == other.ToColumn;
  }
};

template <>
struct boost::hash<RelationshipEdge> {
  [[nodiscard]] auto operator()(
    const RelationshipEdge& edge
  ) const noexcept -> std::size_t {
    std::size_t seed = std::hash<TableId>()(edge.FromColumn);
    boost::hash_combine(seed, std::hash<TableId>()(edge.ToColumn));
    return seed;
  }
};

struct PersistentAppData {
  std::unordered_map<TableId, ImVec2> TableNodePositions;

  template <class Archive>
  void serialize(Archive& archive, const unsigned int /*version*/) {
    archive & TableNodePositions;
  }
};


struct Relationship {
  OnDeleteOnUpdateAction OnUpdateAction;
  OnDeleteOnUpdateAction OnDeleteAction;
};

enum class RelationshipType : uint8_t {
  NoRelationship,
  OutboundRelationship,
  InboundRelationship,
};

struct Column {
  std::string Name;
  std::string Type;
  bool NotNull;
  bool IsPrimaryKey;
  RelationshipType RelationshipType = RelationshipType::NoRelationship;
};

struct Table {
  std::string Name;
  std::vector<ColumnId> Columns;
};

class DbState {
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

class DbController {
public:
  explicit DbController(SQLite::Database&& database) noexcept
      : db_(std::move(database)) {}

  explicit DbController(const std::filesystem::path& dbPath) noexcept
      : db_(SQLite::Database{dbPath.string(), SQLite::OPEN_READWRITE}) {}

  [[nodiscard]] auto DbState() noexcept -> const DbState& {
    return *dbStatePointer_.load(std::memory_order_acquire);
  }

  void UpdateState() {
    frontDbState_ = FetchState();
  }

  [[nodiscard]] auto FetchState() -> class DbState {
    class DbState new_state;
    std::size_t object_id = 0;

    const auto update_columns_for_table = [&](TableId table_id) {
      // This we have to go through each table... So annoying.
      SQLite::Statement query(
        db_,
        std::format("PRAGMA table_info({});", 
                    new_state.tableNames_.left.at(table_id)));

      std::vector<ColumnId> column_ids;
      while (query.executeStep()) {
        const std::string column_name = query.getColumn(1).getString();
        const std::string column_type = query.getColumn(2).getString();
        const bool not_null = static_cast<bool>(query.getColumn(3).getInt());
        // TODO(marko): Handle default value.
        const bool is_primary_key =
          static_cast<bool>(query.getColumn(5).getInt());

        new_state.columns_.emplace(
          object_id,
          Column{
            .Name = column_name,
            .Type = column_type,
            .NotNull = not_null,
            .IsPrimaryKey = is_primary_key,
            // Assume that the column is not a foreign key at this point. This
            // will be updated later when we process the foreign key
            // relationships.
            .RelationshipType = RelationshipType::NoRelationship,
          }
        );

        new_state.columnMemberships_.insert({ object_id, table_id });

        column_ids.push_back(object_id);

        ++object_id;
      }

      return column_ids;
    };


    const auto update_tables = [&]() {
      SQLite::Statement query(
        db_,
        "SELECT name FROM sqlite_master WHERE type='table';"
      );

      while (query.executeStep()) {
        const auto table_name = query.getColumn(0).getString();
        const std::size_t table_id = object_id++;

        new_state.tableNames_.insert({ table_id, table_name });
        new_state.tables_.emplace(
          table_id,
          Table {
            .Name = table_name,
            .Columns = update_columns_for_table(table_id),
          }
        );
      }
    };

    const auto update_relationships = [&]() {
      // This we have to go through each table... So annoying.
      for (const auto& [from_table_id, from_table] : new_state.tables_) {
        const std::string from_table_name = from_table.Name;

        SQLite::Statement query(
          db_,
          std::format("PRAGMA foreign_key_list('{}');", from_table_name)
        );

        while (query.executeStep()) {
          const std::string to_table_name = query.getColumn(2).getString();
          const std::size_t to_table_id =
            new_state.tableNames_.right.at(to_table_name);
          const Table& to_table = new_state.tables_.at(to_table_id);

          // Gotta be careful about these from_column and to_columns. They are
          // column IDs, relative to the table they are in, not the absolute
          // IDs we've been using so far.
          const std::string column_name_from = query.getColumn(3).getString();
          const std::string column_name_to = query.getColumn(4).getString();

          // We have to go and find the column IDs for these names. Luckily, we
          // already know the from-table and to-table values, so we can search
          // for the column within those.
          const ColumnId from_column_id =
            findColumnId(new_state, column_name_from, from_table);
          const ColumnId to_column_id =
            findColumnId(new_state, column_name_to, to_table);

          const auto on_update_action =
              OnDeleteOnUpdateActionFromString(query.getColumn(5).getString());
          const auto on_delete_action =
              OnDeleteOnUpdateActionFromString(query.getColumn(6).getString());

          new_state.relationships_.emplace(
            RelationshipEdge{
              .FromColumn = from_column_id,
              .ToColumn = to_column_id,
            },
            Relationship{
              .OnUpdateAction = on_update_action,
              .OnDeleteAction = on_delete_action
            }
          );

          // Another thing we should do is mark the columns as foreign key.
          // Note that the terminology "from" here actually refers to the table
          // which contains the foreign key.
          //
          // To make that clearer, consider the following pseudo-SQL to help
          // elucidate what from and to mean in this context:
          //
          // CREATE TABLE from (id, to_id);
          // CREATE TABLE to (id);
          new_state.columns_.at(from_column_id).RelationshipType = 
            RelationshipType::InboundRelationship;

          // Now we do something similar for the "to" column, which is the
          // opposite. Note that we have to check if the column already has a
          // relationship since we do not currently support bidirectional
          // relationships.
          // TODO(marko): Support bidirectional relationships.
          if (new_state.columns_.at(to_column_id).RelationshipType ==
              RelationshipType::InboundRelationship) {
            throw std::runtime_error(
              std::format("Column '{}' in table '{}' already has an inbound "
                          "relationship, cannot add outbound relationship.",
                          column_name_to, to_table_name));
          }
          new_state.columns_.at(to_column_id).RelationshipType =
            RelationshipType::OutboundRelationship;
        }
      }
    };

    new_state.path_ = db_.getFilename();
    update_tables();
    update_relationships();

    return new_state;
  }

  void SavePersistentAppData(const PersistentAppData& data) {
    SQLite::Statement createQuery(db_, "CREATE TABLE IF NOT EXISTS __imsql_data"
                                       "(id INTEGER PRIMARY KEY, value TEXT);");

    createQuery.exec();

    std::ostringstream serialized_data;
    {
      boost::archive::text_oarchive archive(serialized_data);
      archive << data;
    }

    SQLite::Statement insertQuery(
      db_,
      "INSERT OR REPLACE INTO __imsql_data (id, value) VALUES (1, ?);"
    );

    insertQuery.bind(1, serialized_data.str());
    insertQuery.exec();
  }

  [[nodiscard]] auto LoadPersistentAppData() -> std::optional<PersistentAppData> {
    try {
      SQLite::Statement query(
        db_,
        "SELECT value FROM __imsql_data LIMIT 1;"
      );

      PersistentAppData data;
      if (query.executeStep()) {
        const std::string serialized_data = query.getColumn(0).getString();
        std::istringstream serialized_stream(serialized_data);
        boost::archive::text_iarchive archive(serialized_stream);
        archive >> data;
      }

      return data;
    } catch (const SQLite::Exception& e) {
      // If the table does not exist, we return an empty optional.
      // TODO(marko): magic number
      if (e.getErrorCode() == 1) {
        return std::nullopt;
      }
      throw; // Re-throw other exceptions.
    }
  }

private:
  [[nodiscard]] auto findColumnId(
    const class DbState& dbState,
    std::string_view columnName,
    const Table& table
  ) -> ColumnId {
    const auto column_id_iter = std::ranges::find_if(
      table.Columns.cbegin(),
      table.Columns.cend(),
      [&] (ColumnId column_id) {
        return dbState.columns_.at(column_id).Name == columnName;
      });
    BOOST_CONTRACT_ASSERT(column_id_iter != table.Columns.cend());

    return *column_id_iter;
  }

  SQLite::Database db_;

  class DbState frontDbState_;
  std::atomic<const class DbState*> dbStatePointer_ = {&frontDbState_};
};

#endif // IMSQLITE_DB_DB_HPP
