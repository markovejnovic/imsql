#ifndef IMSQLITE_VIEWS_DESIGNER_GRAPH_VIEW_HPP
#define IMSQLITE_VIEWS_DESIGNER_GRAPH_VIEW_HPP

#include "immm/render_ctx.hpp"
#include "models/dg/dg.hpp"
#include "models/theme_model.hpp"

namespace imsql {

struct DesignerGraphView {
  DesignerGraphView(imsql::dg::DesignGraphModel& model, ThemeModel& themeModel)
    : model_(&model),
      themeModel_(&themeModel) {}

  /// @brief Render the designer graph view.
  void operator()(immm::RenderCtx& ctx);

private:
  dg::DesignGraphModel* model_;
  ThemeModel* themeModel_;

  bool hasPainted_ = false;

  void renderEditorNodes(immm::RenderCtx& ctx);
};

} // namespace imsql

#endif // IMSQLITE_VIEWS_DESIGNER_GRAPH_VIEW_HPP
