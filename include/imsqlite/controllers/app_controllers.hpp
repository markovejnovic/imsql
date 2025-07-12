#ifndef IMSQLITE_CONTROLLERS_APP_CONTROLLERS_HPP
#define IMSQLITE_CONTROLLERS_APP_CONTROLLERS_HPP

#include "imsqlite/controllers/db.hpp"
#include "imsqlite/controllers/spreadsheet.hpp"
#include "imsqlite/controllers/theme.hpp"
#include "imsqlite/pch/std.hpp"

namespace imsql::controllers {

struct AppControllers {
  explicit AppControllers(
      Db dbController,
      Spreadsheet spreadsheetController
  ) noexcept
      : Db(std::move(dbController)) {}

  Db Db;
  Theme Theme;
};

} // namespace imsql::controllers

#endif // IMSQLITE_CONTROLLERS_APP_CONTROLLERS_HPP
