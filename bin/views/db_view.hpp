#ifndef IMSQLITE_VIEWS_MAIN_WINDOW_HPP
#define IMSQLITE_VIEWS_MAIN_WINDOW_HPP

#include "models/db_model.hpp"
#include "immm/render_ctx.hpp"
#include "models/dg/dg.hpp"
#include "views/designer_graph_view.hpp"

namespace imsql {

/// @brief Represents the window rendered for each open database.
struct DbView {
public:
  explicit DbView(DbModel& dbModel, dg::DesignGraphModel& dgModel)
    : dbModel_(&dbModel), designerView_(dgModel) {}

  /// @brief Render the main window of the application.
  void operator()(immm::RenderCtx& ctx);

private:
  DbModel* dbModel_;

  DesignerGraphView designerView_;
};

} // namespace imsql

#endif // IMSQLITE_VIEWS_MAIN_WINDOW_HPP
