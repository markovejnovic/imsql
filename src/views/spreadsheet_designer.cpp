#include "imsqlite/dsalg/fp.hpp"
#include "imsqlite/math/interval.hpp"
#include "imsqlite/models/base_types.hpp"
#include "imsqlite/models/db.hpp"
#include "imsqlite/models/dg/dg.hpp"
#include "imsqlite/models/dg/transform_node.hpp"
#include "imsqlite/presenters/designer_presenter.hpp"
#include "imsqlite/ui/base_types.hpp"
#include "imsqlite/ui/button.hpp"
#include "imsqlite/ui/dnd.hpp"
#include "imsqlite/ui/nodes/bidirectional_attribute.hpp"
#include "imsqlite/ui/nodes/editor.hpp"
#include "imsqlite/ui/nodes/input_attribute.hpp"
#include "imsqlite/ui/nodes/output_attribute.hpp"
#include "imsqlite/ui/nodes/link.hpp"
#include "imsqlite/ui/nodes/node.hpp"
#include "imsqlite/ui/nodes/title_bar.hpp"
#include "imsqlite/ui/render_ctx.hpp"
#include "imsqlite/ui/text.hpp"
#include "imsqlite/views/spreadsheet_designer.hpp"
#include "imsqlite/pch/std.hpp"
#include "imsqlite/ui/tooltip.hpp"
#include <boost/contract/assert.hpp>
#include <variant>

namespace imsql::views {

namespace {

struct AttrIdInput {
  int Id;
  explicit AttrIdInput(const models::dg::DG::VertexType& vtx) : Id(([&]() {
    const auto vtx_id = gsl::narrow<unsigned int>(vtx);
    const auto signed_vtx_id = gsl::narrow<int>(vtx);
    return signed_vtx_id;
  })()) {}
};

struct AttrIdOutput {
  int Id;
  explicit AttrIdOutput(const models::dg::DG::VertexType& vtx) : Id(([&]() {
    const auto vtx_id = gsl::narrow<unsigned int>(vtx);
    const auto signed_vtx_id = gsl::narrow<int>(vtx);
    return -signed_vtx_id;
  })()) {}
};
struct AttrIdBidirectional {
  AttrIdInput IdIn;
  AttrIdOutput IdOut;

  explicit AttrIdBidirectional(
    const models::dg::DG::VertexType& vtx
  ) : IdIn(vtx), IdOut(vtx) {}
};

using AttrId = std::variant<AttrIdInput, AttrIdOutput, AttrIdBidirectional>;

/// @brief Map an attribute ID to a vertex ID.
auto AttrIdToVertex(int attrId) -> models::dg::DG::VertexType {
  return std::abs(attrId);
}

constexpr auto VertexToImNodeId(const models::dg::DG& dg,
                                const models::dg::DG::VertexType& vtx) -> AttrId {
  const auto vtx_direction = dg.GetVertexDirection(vtx);

  switch (vtx_direction) {
    case models::dg::DG::VertexDirection::Input:
      return AttrIdInput{ vtx };
    case models::dg::DG::VertexDirection::Output:
      return AttrIdOutput{ vtx };
    case models::dg::DG::VertexDirection::Bidirectional: {
      return AttrIdBidirectional{ vtx };
    }
  }
}

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
      std::visit(
        dsalg::Overloads {
          [&](const AttrIdInput& attr) {
            ui::nodes::InputAttribute input_attr{ctx, attr.Id};
            ui::Text vtx_name_text{ctx, std::format("{} ({})", dg.VertexName(vtx), attr.Id)};
          },
          [&](const AttrIdOutput& attr) {
            ui::nodes::OutputAttribute output_attr{ctx, attr.Id};
            ui::Text vtx_name_text{ctx, std::format("{} ({})", dg.VertexName(vtx), attr.Id)};
          },
          [&](const AttrIdBidirectional& attr) {
            ui::nodes::BidirectionalAttribute bidirectional_attr{
              ctx,
              attr.IdIn.Id, [&]() {
                ui::Text vtx_name_text{ctx, std::format("{} ({} -> {})", dg.VertexName(vtx),
                                                        attr.IdIn.Id, attr.IdOut.Id)};
              },
              attr.IdOut.Id, []() {},
            };
          },
        },
        VertexToImNodeId(dg, vtx)
      );
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

    ui::nodes::Link link_node{ctx, dg.GetId(edge),
                              AttrIdOutput(src_vtx).Id, AttrIdInput(tgt_vtx).Id};
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
      // We need to convert the IDs back 
      printf("Connecting attributes %d and %d\n",
             link_created->first, link_created->second);
      printf("Converted to vertices %d and %d\n",
             AttrIdToVertex(link_created->first),
             AttrIdToVertex(link_created->second));
      presenter.AddEdge(AttrIdToVertex(link_created->first), AttrIdToVertex(link_created->second));
    }

    presenter.OnPaint();
  }
}

} // namespace imsql::views
