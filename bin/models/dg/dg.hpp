#ifndef IMSQL_MODELS_DG_DESIGN_GRAPH_MODEL_HPP
#define IMSQL_MODELS_DG_DESIGN_GRAPH_MODEL_HPP

#include "models/db_model.hpp"
#include "models/dg/nodes/transformer_node.hpp"
#include "models/dg/nodes/base_node.hpp"
#include "models/dg/nodes/spreadsheet_node.hpp"
#include "models/dg/graph_types.hpp"
#include "pch.hpp"
#include <memory>

namespace imsql::dg {

/// @brief Represents the design graph, ie. the graph you see in the "Designer" tab of the GUI.
class DesignGraphModel {
public:
  explicit DesignGraphModel(DbModel* dbModel);

private:
  GraphType graph_;

  /// @brief Manages the lifetime of nodes in the design graph.
  /// @note It is unsafe to store Node's in the vector itself. It may resize which will invalidate
  ///       the pointers to the nodes. Note that nodeProperties_, for one, holds pointers to these
  /// @todo Profile std::vector of std::unique_ptr<Node> vs. std::queue of Node.
  std::vector<std::unique_ptr<BaseNode>> nodesPool_;

  /// @brief Maps nodes to their properties.
  boost::unordered_map<const BaseNode*, NodeProperty> nodeProperties_;

  // @note Order matters here. We need nextObjectId_ to be the first member so that it gets
  // initialized to zero before spreadsheetNode_ is initialized.
  int nextObjectId_ = 0;
  DbModel *dbModel_;
  SpreadsheetNode* spreadsheetNode_;
public:
  [[nodiscard]] constexpr auto GetSpreadsheetNode() noexcept -> SpreadsheetNode* {
    return spreadsheetNode_;
  }

  [[nodiscard]] constexpr auto Nodes() const noexcept {
    return nodesPool_
      | std::views::transform([](const auto& node) { return node.get(); });
  }

  [[nodiscard]] constexpr auto Vertices(const BaseNode* node) const noexcept {
    return nodeProperties_.at(node).Vertices;
  }

  [[nodiscard]] constexpr auto GetVertexDirection(VertexType vtx) const noexcept {
    return graph_[vtx].Direction;
  }

  [[nodiscard]] constexpr auto VertexName(VertexType vtx) const noexcept -> std::string_view {
    return graph_[vtx].Vertex->Name();
  }

  [[nodiscard]] constexpr auto VertexNode(VertexType vtx) const noexcept {
    return graph_[vtx].Vertex.get();
  }

  [[nodiscard]] constexpr auto EdgeRange() const noexcept {
    return boost::edges(graph_);
  }

  [[nodiscard]] constexpr auto EdgeSource(const EdgeType& edge) const noexcept {
    return boost::source(edge, graph_);
  }

  [[nodiscard]] constexpr auto EdgeTarget(const EdgeType& edge) const noexcept {
    return boost::target(edge, graph_);
  }

  [[maybe_unused]] constexpr auto AddSpreadsheetColumn(std::string_view column_name) -> VertexType {
    return MakeVertex(std::make_shared<SpreadsheetVertex>(this, std::string(column_name)),
                      VertexDirection::Input, spreadsheetNode_);
  }

  [[nodiscard]] auto GetRowIds() const noexcept -> std::vector<std::size_t>;

  [[maybe_unused]] constexpr auto AddTransformNode() -> TransformNode* {
    TransformNode* node_ptr = AddEmptyNode(TransformNode{this});
    auto keyip = MakeVertex(std::make_shared<TransformInputKeyVertex>(this, node_ptr),
                            VertexDirection::Input, node_ptr);
    auto valip = MakeVertex(std::make_shared<TransformInputValueVertex>(this, node_ptr),
                            VertexDirection::Input, node_ptr);
    auto valout = MakeVertex(std::make_shared<TransformOutputVertex>(this, node_ptr),
                             VertexDirection::Output, node_ptr);
    // @todo(marko): This dynamic_cast is a 1AM hack.
    node_ptr->SetInputKeyVertex(
      dynamic_cast<TransformInputKeyVertex*>(graph_[keyip].Vertex.get()));
    node_ptr->SetInputValueVertex(
      dynamic_cast<TransformInputValueVertex*>(graph_[valip].Vertex.get()));
    node_ptr->SetOutputVertex(
      dynamic_cast<TransformOutputVertex*>(graph_[valout].Vertex.get()));
    return node_ptr;
  }

  constexpr void AddEdge(VertexType source, VertexType target) {
    MakeEdge(source, target);
  }

  [[nodiscard]] constexpr auto Vertex(const VertexType& vtx) const -> const BaseVertex* {
    return graph_[vtx].Vertex.get();
  }

  [[nodiscard]] constexpr auto Vertex(const VertexType& vtx) -> BaseVertex* {
    return graph_[vtx].Vertex.get();
  }

  [[nodiscard]] constexpr auto InputVertex(const VertexType& vtx) const -> const BaseVertex* {
    auto in_edges_range = boost::in_edges(vtx, graph_);
    if (in_edges_range.first == in_edges_range.second) {
      return nullptr; // no predecessor
    }

    BOOST_CONTRACT_ASSERT(std::distance(in_edges_range.first, in_edges_range.second) == 1);

    const auto source_vtx = boost::source(*in_edges_range.first, graph_);
    return graph_[source_vtx].Vertex.get();
  }

  [[nodiscard]] constexpr auto GetId(const EdgeType& edge) const noexcept -> int {
    return graph_[edge].Id;
  }

  [[nodiscard]] constexpr auto GetId(const BaseNode* node) const noexcept -> int {
    return nodeProperties_.at(node).NodeId;
  }

  [[maybe_unused]] constexpr auto MakeVertex(
    const std::shared_ptr<BaseVertex>& vertex,
    VertexDirection direction,
    BaseNode* node
  ) -> VertexType {
    auto vtx = boost::add_vertex(VertexProperty{
      .Vertex = vertex,
      .Direction = direction,
    }, graph_);

    nodeProperties_.at(node).Vertices.push_back(vtx);

    graph_[vtx].Vertex->SetVertexId(vtx);

    return vtx;
  }

private:
  /// @brief Register a node with the system which has no vertices associated with it.
  template <class T>
  constexpr auto AddEmptyNode(T&& node) -> T* {
    auto node_smart =  std::make_unique<T>(std::forward<T>(node));
    T* node_ptr = node_smart.get();

    nodesPool_.emplace_back(std::move(node_smart));
    nodeProperties_.emplace(node_ptr, NodeProperty{
      .Vertices = {},
      .NodeId = nextObjectId_++,
    });

    return node_ptr;
  }

  auto AddSpreadsheetNode() -> SpreadsheetNode* {
    auto* node_ptr = AddEmptyNode(SpreadsheetNode{this});
    MakeVertex(std::make_shared<SpreadsheetVertex>(this, "id"), VertexDirection::Input, node_ptr);
    return node_ptr;
  }

  [[maybe_unused]] constexpr auto MakeEdge(VertexType source, VertexType target) -> EdgeType {
    return boost::add_edge(source, target, EdgeProperty{.Id = nextObjectId_++}, graph_).first;
  }
};

} // namespace imsql::dg

#endif // IMSQL_MODELS_DG_DESIGN_GRAPH_MODEL_HPP
