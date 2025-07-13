#include "util/fp.hpp"
#include "util/interval.hpp"
#include "models/dg/dg.hpp"
#include "models/dg/transform_node.hpp"
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

void editorNodes(immm::RenderCtx& ctx, const dg::DesignGraphModel& dg) {

  // For each vertex in the graph, we need to render its node, and the vertices enclosed in the
  // node.
  for (const auto* node : dg.Nodes()) {
    const int node_id = dg.GetId(node);
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
      model.AddOperatorNode<dg::TransformNode>();
    }
  }
}

} // namespace

void DesignerGraphView::operator()(immm::RenderCtx& ctx) {
  {
    toolbar(ctx, *model_);

    {
      immm::nodes::Editor editor{ctx};
      editorNodes(ctx, *model_);
      editorEdges(ctx, *model_);
    }

    std::optional link_created = immm::nodes::Link::WhichLinkCreated();
    if (link_created.has_value()) {
      // We need to convert the IDs back 
      model_->AddEdge(AttrIdToVertex(link_created->first), AttrIdToVertex(link_created->second));
    }

    hasPainted_ = true;
  }
}

} // namespace imsql
