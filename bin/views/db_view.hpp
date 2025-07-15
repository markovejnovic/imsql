#ifndef IMSQLITE_VIEWS_MAIN_WINDOW_HPP
#define IMSQLITE_VIEWS_MAIN_WINDOW_HPP

#include "models/db_model.hpp"
#include "immm/render_ctx.hpp"
#include "models/dg/dg.hpp"
#include "models/theme_model.hpp"
#include "views/designer_graph_view.hpp"
#include "views/spreadsheet_table_view.hpp"

namespace imsql {

/// @brief Represents the window rendered for each open database.
struct DbView {
public:
  explicit DbView(DbModel& dbModel, dg::DesignGraphModel& dgModel,
                  ThemeModel& themeModel)
    : dbModel_(&dbModel),
      themeModel_(&themeModel),
      SpreadsheetTableView_(dgModel, themeModel),
      designerView_(dgModel, themeModel) {}

  /// @brief Render the main window of the application.
  void operator()(immm::RenderCtx& ctx);

private:
  DbModel* dbModel_;
  ThemeModel* themeModel_;

  SpreadsheetTableView SpreadsheetTableView_;
  DesignerGraphView designerView_;
};

} // namespace imsql

#endif // IMSQLITE_VIEWS_MAIN_WINDOW_HPP
