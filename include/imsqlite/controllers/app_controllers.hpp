#ifndef IMSQLITE_CONTROLLERS_APP_CONTROLLERS_HPP
#define IMSQLITE_CONTROLLERS_APP_CONTROLLERS_HPP

#include "imsqlite/controllers/db.hpp"
#include "imsqlite/controllers/spreadsheet.hpp"
#include "imsqlite/controllers/theme.hpp"
#include "imsqlite/dsalg/views/dedup.hpp"
#include "imsqlite/models/base_types.hpp"
#include "imsqlite/std.hpp"

namespace imsql::controllers {

struct AppControllers {
  explicit AppControllers(
      Db dbController,
      Spreadsheet spreadsheetController
  ) noexcept
      : Db(std::move(dbController)),
        Sheets(std::move(spreadsheetController)) {}

  Db Db;
  Spreadsheet Sheets;
  Theme Theme;

  constexpr void LinkBetweenDbAndMainSheet(models::ColumnId dbLink,
                                           models::ColumnId mainsheetLink) {
    if (!Db.CModel().ColumnExists(dbLink)) {
      throw std::runtime_error("Db link column does not exist: " + imsql::strof(dbLink));
    }

    if (!Sheets.CModel().ColumnExists(mainsheetLink)) {
      throw std::runtime_error("Main sheet link column does not exist: " +
                               imsql::strof(mainsheetLink));
    }

    const models::RelationshipEdge edge = {
      .FromColumn = dbLink,
      .ToColumn = mainsheetLink,
    };

    crossRelationships_.emplace(dbLink, edge);
    crossRelationships_.emplace(mainsheetLink, edge);
  }

  [[nodiscard]] constexpr auto CrossRelationships() const noexcept {
    return crossRelationships_ | std::views::values | imsql::vw::dedup;
  }

  constexpr void LinkWithinDb(models::ColumnId lhs,
                              models::ColumnId rhs) {
    if (!Db.CModel().ColumnExists(lhs)) {
      throw std::runtime_error("LHS column does not exist: " + imsql::strof(lhs));
    }

    if (!Db.CModel().ColumnExists(rhs)) {
      throw std::runtime_error("RHS column does not exist: " + imsql::strof(rhs));
    }

    const models::RelationshipEdge edge = {
      .FromColumn = lhs,
      .ToColumn = rhs,
    };

    crossRelationships_.emplace(lhs, edge);
    crossRelationships_.emplace(rhs, edge);
  }

private:
  // TODO(marko): Man this data model sucks balls. I really wish I had made a system unified across
  // sheets and the database. Some sort of abstraction layer would be great.
  //
  // It is what it is.
  boost::unordered_map<models::ColumnId, models::RelationshipEdge> crossRelationships_;
};

} // namespace imsql::controllers

#endif // IMSQLITE_CONTROLLERS_APP_CONTROLLERS_HPP
