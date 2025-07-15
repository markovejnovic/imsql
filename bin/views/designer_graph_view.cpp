#include "immm/nodes/color_style.hpp"
#include "models/dg/nodes.hpp"
#include "util/fp.hpp"
#include "util/interval.hpp"
#include "models/dg/dg.hpp"
#include "immm/button.hpp"
#include "immm/nodes/bidirectional_attribute.hpp"
#include "immm/nodes/editor.hpp"
#include "immm/nodes/input_attribute.hpp"
#include "immm/nodes/output_attribute.hpp"
#include "immm/nodes/link.hpp"
#include "immm/nodes/node.hpp"
#include "immm/nodes/title_bar.hpp"
#include "immm/render_ctx.hpp"
#include "immm/text.hpp"
#include "designer_graph_view.hpp"
#include "pch.hpp"
#include <boost/contract/assert.hpp>
#include <variant>

namespace imsql {

namespace {

struct AttrIdInput {
  int Id;
  explicit AttrIdInput(const dg::DesignGraphModel::VertexType& vtx) : Id(([&]() {
    const auto vtx_id = gsl::narrow<unsigned int>(vtx);
    const auto signed_vtx_id = gsl::narrow<int>(vtx);
    return signed_vtx_id;
  })()) {}
};

struct AttrIdOutput {
  int Id;
  explicit AttrIdOutput(const dg::DesignGraphModel::VertexType& vtx) : Id(([&]() {
    const auto vtx_id = gsl::narrow<unsigned int>(vtx);
    const auto signed_vtx_id = gsl::narrow<int>(vtx);
    return -signed_vtx_id;
  })()) {}
};
struct AttrIdBidirectional {
  AttrIdInput IdIn;
  AttrIdOutput IdOut;

  explicit AttrIdBidirectional(
    const dg::DesignGraphModel::VertexType& vtx
  ) : IdIn(vtx), IdOut(vtx) {}
};

using AttrId = std::variant<AttrIdInput, AttrIdOutput, AttrIdBidirectional>;

/// @brief Map an attribute ID to a vertex ID.
auto AttrIdToVertex(int attrId) -> dg::DesignGraphModel::VertexType {
  return std::abs(attrId);
}

constexpr auto VertexToImNodeId(const dg::DesignGraphModel& dg,
                                const dg::DesignGraphModel::VertexType& vtx) -> AttrId {
  const auto vtx_direction = dg.GetVertexDirection(vtx);

  switch (vtx_direction) {
    case dg::DesignGraphModel::VertexDirection::Input:
      return AttrIdInput{ vtx };
    case dg::DesignGraphModel::VertexDirection::Output:
      return AttrIdOutput{ vtx };
    case dg::DesignGraphModel::VertexDirection::Bidirectional: {
      return AttrIdBidirectional{ vtx };
    }
  }
}


void editorEdges(immm::RenderCtx& ctx, dg::DesignGraphModel& dg) {
  // Now we need to render the links between the nodes.
  auto [ei, ei_end] = dg.EdgeRange();
  for (; ei != ei_end; ++ei) {
    const auto& edge = *ei;
    auto src_vtx = dg.EdgeSource(edge);
    auto tgt_vtx = dg.EdgeTarget(edge);

    immm::nodes::Link link_node{ctx, dg.GetId(edge),
                              AttrIdOutput(src_vtx).Id, AttrIdInput(tgt_vtx).Id};
  }
}

void toolbar(immm::RenderCtx& ctx, dg::DesignGraphModel& model) {
  // Render the toolbar with a button to add a new node.
  // TODO(marko): Implement this, obviously.
  {
    auto [button, clicked] = immm::Button::CreateAndIsClicked(ctx, "Transformer Node");
    if (clicked) {
      model.AddTransformNode();
    }
  }
}

} // namespace

void DesignerGraphView::operator()(immm::RenderCtx& ctx) {
  {
    toolbar(ctx, *model_);

    {
      immm::nodes::Editor editor{ctx};
      renderEditorNodes(ctx);
      editorEdges(ctx, *model_);
    }

    std::optional link_created = immm::nodes::Link::WhichLinkCreated();
    if (link_created.has_value()) {
      // We need to convert the IDs back 
      model_->AddEdge(AttrIdToVertex(link_created->first), AttrIdToVertex(link_created->second));
    }

    std::optional link_destroyed = immm::nodes::Link::WhichLinkDestroyed();
    if (link_destroyed.has_value()) {
      printf("Link destroyed: %d\n", *link_destroyed);
    }

    hasPainted_ = true;
  }
}

void DesignerGraphView::renderEditorNodes(immm::RenderCtx& ctx) {
  auto& dg = *model_;

  // For each vertex in the graph, we need to render its node, and the vertices enclosed in the
  // node.
  for (const auto* node : dg.Nodes()) {
    const int node_id = dg.GetId(node);

    const auto color_style = std::visit(
      util::Overloads {
        [&](const dg::DbNode&) {
          return immm::nodes::MakeColorStyle(
            ImNodesCol_TitleBar, themeModel_->Nodes.Database.TitleBar.Default,
            ImNodesCol_TitleBarSelected, themeModel_->Nodes.Database.TitleBar.Selected,
            ImNodesCol_TitleBarHovered, themeModel_->Nodes.Database.TitleBar.Hover
          );
        },
        [&](const dg::OperatorNode&) {
          return immm::nodes::MakeColorStyle(
            ImNodesCol_TitleBar, themeModel_->Nodes.Operator.TitleBar.Default,
            ImNodesCol_TitleBarSelected, themeModel_->Nodes.Operator.TitleBar.Selected,
            ImNodesCol_TitleBarHovered, themeModel_->Nodes.Operator.TitleBar.Hover
          );
        },
        [&](const dg::SpreadsheetNode&) {
          // TODO(marko): Color the
          return immm::nodes::MakeColorStyle(
            ImNodesCol_TitleBar, themeModel_->Nodes.Spreadsheet.TitleBar.Default,
            ImNodesCol_TitleBarSelected, themeModel_->Nodes.Spreadsheet.TitleBar.Selected,
            ImNodesCol_TitleBarHovered, themeModel_->Nodes.Spreadsheet.TitleBar.Hover
          );
        },
      },
      node->Variant()
    );

    immm::nodes::Node node_view{ctx, node_id};

    // TODO(marko): Implement loading node positions from the database.

    { immm::nodes::TitleBar title_bar{ctx, std::format("{} ({})", node->Name(), node_id)}; }

    for (const auto& vtx : dg.Vertices(node)) {
      static_assert(std::is_integral_v<std::remove_reference_t<decltype(vtx)>>, 
                    "Vertex type must be integral for the node ID");
      std::visit(
        util::Overloads {
          [&](const AttrIdInput& attr) {
            immm::nodes::InputAttribute input_attr{ctx, attr.Id};
            immm::Text vtx_name_text{ctx, std::format("{} ({})", dg.VertexName(vtx), attr.Id)};
          },
          [&](const AttrIdOutput& attr) {
            immm::nodes::OutputAttribute output_attr{ctx, attr.Id};
            immm::Text vtx_name_text{ctx, std::format("{} ({})", dg.VertexName(vtx), attr.Id)};
          },
          [&](const AttrIdBidirectional& attr) {
            immm::nodes::BidirectionalAttribute bidirectional_attr{
              ctx,
              attr.IdIn.Id, [&]() {
                immm::Text vtx_name_text{ctx, std::format("{} ({} -> {})", dg.VertexName(vtx),
                                                        attr.IdIn.Id, attr.IdOut.Id)};
              },
              attr.IdOut.Id, []() {},
            };
          },
        },
        VertexToImNodeId(dg, vtx)
      );
    }

    // Conditionally, we need to add a button to enable the user to add a new vertex to the node.
    if (std::holds_alternative<dg::SpreadsheetNode>(node->Variant())) {
      const auto [button, isClicked] = immm::Button::CreateAndIsClicked(ctx, "Add Vertex");
      if (isClicked) {
        dg.AddSpreadsheetColumn("unnamed column");
      }
    }
  }
}

} // namespace imsql
