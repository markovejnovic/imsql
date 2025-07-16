#include "base_node.hpp"
#include "models/dg/dg.hpp"

namespace imsql::dg {

BaseVertex::BaseVertex(DesignGraphModel* graph, VertexType vertexId)
  : graph_(graph), vertexId_(vertexId) {
  // Initialize the vertex ID in the graph.
}

auto BaseVertex::GetInputVertex() const noexcept -> const BaseVertex* {
  return graph_->InputVertex(vertexId_);
}

auto BaseNode::Vertices() const noexcept -> std::vector<const BaseVertex*> {
  return std::ranges::to<std::vector<const BaseVertex*>>(
    graph()->Vertices(this)
      | std::views::transform([this](const VertexType& vtx) {
          return graph()->Vertex(vtx);
        })
  );
}

} // namespace imsql::dg
