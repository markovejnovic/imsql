#include "imsqlite/math/interval.hpp"
#include "imsqlite/models/base_types.hpp"
#include "imsqlite/models/db.hpp"
#include "imsqlite/models/dg/dg.hpp"
#include "imsqlite/models/dg/transform_node.hpp"
#include "imsqlite/presenters/designer_presenter.hpp"
#include "imsqlite/ui/base_types.hpp"
#include "imsqlite/ui/button.hpp"
#include "imsqlite/ui/dnd.hpp"
#include "imsqlite/ui/nodes/editor.hpp"
#include "imsqlite/ui/nodes/input_attribute.hpp"
#include "imsqlite/ui/nodes/link.hpp"
#include "imsqlite/ui/nodes/node.hpp"
#include "imsqlite/ui/nodes/output_attribute.hpp"
#include "imsqlite/ui/nodes/title_bar.hpp"
#include "imsqlite/ui/render_ctx.hpp"
#include "imsqlite/ui/text.hpp"
#include "imsqlite/views/spreadsheet_designer.hpp"
#include "imsqlite/pch/std.hpp"
#include "imsqlite/ui/tooltip.hpp"
#include <boost/contract/assert.hpp>

namespace imsql::views {

namespace {

template <class F>
void onlyOnFirstPaint(imsql::presenters::DesignerPresenter& presenter, F&& fxn) {
  // This function is called only once, when the view is first painted.
  if (presenter.IsFirstPaint()) {
    std::invoke(std::forward<F>(fxn));
  }
}

void editorNodes(ui::RenderCtx& ctx, imsql::presenters::DesignerPresenter& presenter) {
  const auto& dg = presenter.Model();

  // For each vertex in the graph, we need to render its node, and the vertices enclosed in the
  // node.
  for (const auto* node : dg.Nodes()) {
    const int node_id = dg.GetId(node);
    ui::nodes::Node node_view{ctx, node_id};

    onlyOnFirstPaint(presenter, [&]() {
      node_view.SetPosition(
        ctx.Controllers().Db.CModel().CPersistentData().TableNodePositions.contains(node_id)
          ? ctx.Controllers().Db.CModel().CPersistentData().TableNodePositions.at(node_id)
          : ui::Vec2{0.0F, 0.0F});
    });

    { ui::nodes::TitleBar title_bar{ctx, std::format("{} ({})", node->Name(), node_id)}; }

    for (const auto& vtx : dg.Vertices(node)) {
      static_assert(std::is_integral_v<std::remove_reference_t<decltype(vtx)>>, 
                    "Vertex type must be integral for the node ID");
      const int vtx_id = dg.GetId(vtx);

      switch (dg.VertexDirection(vtx)) {
        case models::dg::DG::VertexNodeDirection::Input: {
          // The vertex is an input vertex, so we need to render it as an input vertex.
          ui::nodes::InputAttribute input_attr{ctx, vtx_id};
          ui::Text vtx_name_text{ctx, std::format("{} ({})", dg.VertexName(vtx), vtx_id)};
          continue;
        }
        case models::dg::DG::VertexNodeDirection::Output: {
          // The vertex is an output vertex, so we need to render it as an output vertex.
          ui::nodes::OutputAttribute output_attr{ctx, vtx_id};
          ui::Text vtx_name_text{ctx, std::format("{} ({})", dg.VertexName(vtx), vtx_id)};
          continue;
        }
      }
    }
  }
}

void editorEdges(ui::RenderCtx& ctx, const imsql::presenters::DesignerPresenter& presenter) {
  const auto& dg = presenter.Model();

  // Now we need to render the links between the nodes.
  auto [ei, ei_end] = dg.EdgeRange();
  for (; ei != ei_end; ++ei) {
    const auto& edge = *ei;
    auto src_vtx = dg.EdgeSource(edge);
    auto tgt_vtx = dg.EdgeTarget(edge);

    ui::nodes::Link link_node{ctx, dg.GetId(edge), dg.GetId(src_vtx), dg.GetId(tgt_vtx)};
  }
}

struct ToolDndAddition {
  enum class Tool : uint8_t {
    Transformer,
  } Type;
};

void toolbar(ui::RenderCtx& ctx, imsql::presenters::DesignerPresenter& presenter) {
  // Render the toolbar with a button to add a new node.
  // TODO(marko): Implement this, obviously.
  {
    auto [button, clicked] = ui::Button::CreateAndIsClicked(ctx, "Transformer Node");
    if (clicked) {
      presenter.AddOperatorNode<models::dg::TransformNode>();
    }
  }
}

} // namespace

void SpreadsheetDesigner(ui::RenderCtx& ctx, imsql::presenters::DesignerPresenter& presenter) {
  {
    toolbar(ctx, presenter);

    {
      ui::nodes::Editor editor{ctx};
      editorNodes(ctx, presenter);
      editorEdges(ctx, presenter);
    }

    std::optional link_created = ui::nodes::Link::WhichLinkCreated();
    if (link_created.has_value()) {
      presenter.AddEdge(*link_created);
    }

    presenter.OnPaint();
  }
}

} // namespace imsql::views
