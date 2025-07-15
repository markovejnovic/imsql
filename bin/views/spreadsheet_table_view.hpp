#ifndef IMSQL_SPREADSHEET_TABLE_VIEW_HPP
#define IMSQL_SPREADSHEET_TABLE_VIEW_HPP

#include "immm/render_ctx.hpp"
#include "models/dg/dg.hpp"
#include "models/theme_model.hpp"

namespace imsql {

class SpreadsheetTableView {
public:
  SpreadsheetTableView(imsql::dg::DesignGraphModel& model, ThemeModel& themeModel)
    : model_(&model),
      themeModel_(&themeModel) {}

  void operator()(immm::RenderCtx& ctx);

private:
  dg::DesignGraphModel* model_;
  ThemeModel* themeModel_;
};

} // namespace imsql

#endif // IMSQL_SPREADSHEET_TABLE_VIEW_HPP
