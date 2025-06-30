#include "imsqlite/sql/db.hpp"
#include "SQLiteCpp/Database.h"
#include "SQLiteCpp/Statement.h"
#include <memory>

namespace imsql::sql {

auto DbTablesView::iterator::operator*() const -> Table {
  // The name is the first column in the result set.
  return Table{query_.getColumn(0).getString()};
}

auto DbTablesView::iterator::operator++() -> iterator& {
  // Execute the next step in the query.
  haveData_ = query_.executeStep();
  return *this;
}

auto DbTablesView::iterator::operator==(
  std::default_sentinel_t /*end*/
) const -> bool {
  // If we don't have data, we're at the end.
  return !haveData_;
}

auto Db::FromFile(const std::filesystem::path& path) -> Db {
  // TODO(marko): Catch errors
  return Db{SQLite::Database{ path.string(), SQLite::OPEN_READWRITE }};
}

} // namespace imsql::sql
