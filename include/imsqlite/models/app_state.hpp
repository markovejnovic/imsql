#ifndef IMSQLITE_MODELS_APP_STATE_HPP
#define IMSQLITE_MODELS_APP_STATE_HPP

#include "imsqlite/models/db.hpp"
#include "imsqlite/models/views/spreadsheet_designer.hpp"

namespace imsql::models {

struct AppState {
  Db Database;

  struct {
    views::SpreadsheetDesigner SpreadsheetDesigner;
  } ViewStates;
};

} // namespace imsql::models

#endif // IMSQLITE_MODELS_APP_STATE_HPP
