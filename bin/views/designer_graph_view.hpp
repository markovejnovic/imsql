#ifndef IMSQLITE_VIEWS_DESIGNER_GRAPH_VIEW_HPP
#define IMSQLITE_VIEWS_DESIGNER_GRAPH_VIEW_HPP

#include "immm/render_ctx.hpp"
#include "models/dg/dg.hpp"

namespace imsql {

struct DesignerGraphView {
  explicit DesignerGraphView(imsql::dg::DesignGraphModel& model) : model_(&model) {}

  /// @brief Render the designer graph view.
  void operator()(immm::RenderCtx& ctx);

private:
  dg::DesignGraphModel* model_;
  bool hasPainted_ = false;
};

} // namespace imsql

#endif // IMSQLITE_VIEWS_DESIGNER_GRAPH_VIEW_HPP
