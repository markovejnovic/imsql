#include "imsqlite/controllers/spreadsheet.hpp"
#include "imsqlite/models/base_types.hpp"
#include "imsqlite/models/spreadsheet.hpp"

namespace imsql::controllers {

void Spreadsheet::AddNewColumn() {
  model_.columnNames_.emplace(identCtrl_->New<models::ColumnId>(), kDefaultColumnName);
}

} // namespace imsql::controllers
