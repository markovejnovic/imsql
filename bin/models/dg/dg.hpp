#ifndef IMSQL_MODELS_DG_DESIGN_GRAPH_MODEL_HPP
#define IMSQL_MODELS_DG_DESIGN_GRAPH_MODEL_HPP

#include "models/db_model.hpp"
#include "models/dg/nodes.hpp"
#include "pch.hpp"

namespace imsql::dg {

enum class IOMode : uint8_t {
  Input = 0,
  Output = 1,
};

struct VertexDescriptorStructure {
  IOMode Mode : 1;
  unsigned int Id : sizeof(int) - 1;
};

static_assert(sizeof(VertexDescriptorStructure) == sizeof(int),
              "VertexDescriptor must be the same size as int.");

union VertexDescriptor {
  int AsInt;
  VertexDescriptorStructure AsStruct;
};

static_assert(sizeof(VertexDescriptor) == sizeof(int),
              "VertexDescriptor must be the same size as int.");

/// @brief Represents the design graph, ie. the graph you see in the "Designer" tab of the GUI.
class DesignGraphModel {
public:
  explicit DesignGraphModel(DbModel* dbModel);

  enum class VertexDirection : uint8_t {
    Input,
    Output,
    Bidirectional,
  };

  /// @note Each vertex has this metadata attached to it.
  struct VertexProperty {
    std::string Name;
    Node* Node;
    VertexDirection Direction;
  };

  /// @note Each edge has this metadata attached to it.
  struct EdgeProperty {
    int Id;
  };

private:
  boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS,
                        VertexProperty, EdgeProperty> graph_;
public:
  using VertexType = boost::graph_traits<decltype(graph_)>::vertex_descriptor;
  static_assert(std::is_same_v<VertexType, unsigned long>,
                "VertexType must be the same as VertexDescriptor.");
  using EdgeType = boost::graph_traits<decltype(graph_)>::edge_descriptor;

  /// @brief Information associated with each node in the design graph.
  struct NodeProperty {
    std::vector<VertexType> Vertices;
    int NodeId;
  };

private:
  /// @brief Manages the lifetime of nodes in the design graph.
  /// @note It is unsafe to store Node's in the vector itself. It may resize which will invalidate
  ///       the pointers to the nodes. Note that nodeProperties_, for one, holds pointers to these
  /// @todo Profile std::vector of std::unique_ptr<Node> vs. std::queue of Node.
  std::vector<std::unique_ptr<Node>> nodesPool_;

  /// @brief Maps nodes to their properties.
  boost::unordered_map<const Node*, NodeProperty> nodeProperties_;
  boost::unordered_map<int, VertexType> vertexIds_;

  // @note Order matters here. We need nextObjectId_ to be the first member so that it gets
  // initialized to zero before spreadsheetNode_ is initialized.
  int nextObjectId_ = 0;
  DbModel *dbModel_;
  Node* spreadsheetNode_;
public:
  /// @brief Output the design graph in DOT format, useful for debugging.
  void DumpDot(std::ostream& out) const;

  [[nodiscard]] constexpr auto GetSpreadsheetNode() noexcept -> Node* {
    return spreadsheetNode_;
  }

  [[nodiscard]] constexpr auto Nodes() const noexcept {
    return nodesPool_
      | std::views::transform([](const auto& node) { return node.get(); });
  }

  [[nodiscard]] constexpr auto Vertices(const Node* node) const noexcept {
    return nodeProperties_.at(node).Vertices;
  }

  [[nodiscard]] constexpr auto GetVertexDirection(VertexType vtx) const noexcept {
    return graph_[vtx].Direction;
  }

  [[nodiscard]] constexpr auto VertexName(VertexType vtx) const noexcept -> const std::string& {
    return graph_[vtx].Name;
  }

  [[nodiscard]] constexpr auto GetRowIds() const -> std::vector<std::size_t> {
    BOOST_CONTRACT_ASSERT(spreadsheetNode_ != nullptr);

    const auto& spreadsheet_property = nodeProperties_.at(spreadsheetNode_);
    // Let's find the id vertex in the spreadsheet node.
    // @todo Perhaps we should avoid searching for the id vertex every time?
    const VertexType id_vertex = *std::ranges::find_if(
      spreadsheet_property.Vertices,
      [this](const VertexType& vtx) {
        return graph_[vtx].Name == "id";
      }
    );

    // Now we find the edges facing into our id vertex.
    const auto inp_edges = boost::in_edges(id_vertex, graph_);
    if (inp_edges.first == inp_edges.second) {
      return {}; // No edges found, return empty vector.
    }

    // We do not support multiple edges to the id vertex.
    BOOST_CONTRACT_ASSERT(std::distance(inp_edges.first, inp_edges.second) <= 1);

    // Now, let us find the vertex pointing to the id vertex.
    const auto source_vertex = boost::source(*inp_edges.first, graph_);
    const auto& source_vtx_property = graph_[source_vertex];
    // @todo(marko): This cast is a little bit of a hack.
    const auto& source_node = graph_[source_vertex].Node->As<DbTableNode>();

    return source_node.ValuesForColumn<std::size_t>(source_vtx_property.Name);
  }

  /// @brief For the given row ID, retrieve all the column values in the spreadsheet.
  [[nodiscard]] auto GetSpreadsheetValues(std::size_t rowId) const -> std::vector<std::string>;

  [[nodiscard]] constexpr auto VertexNode(VertexType vtx) const noexcept {
    return graph_[vtx].Node;
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
    return MakeVertex(std::string(column_name), spreadsheetNode_, VertexDirection::Input);
  }

  /// @brief Retrieve the unique identifier for the given object.
  /// @{
  [[nodiscard]] auto GetId(const Node* node) const noexcept -> int;
  [[nodiscard]] auto GetId(const EdgeType& edge) const noexcept -> int;
  /// @}

  void AddTransformNode() {
    auto* node_ptr = AddEmptyNode(TransformNode{});
    node_ptr->As<TransformNode>().SetVertices(
      MakeVertex("key", node_ptr, VertexDirection::Input),
      MakeVertex("value", node_ptr, VertexDirection::Input),
      MakeVertex("output", node_ptr, VertexDirection::Output)
    );
  }

  constexpr void AddEdge(VertexType source, VertexType target) {
    MakeEdge(source, target);
  }

private:
  /// @brief Register a node with the system which has no vertices associated with it.
  template <class T>
  constexpr auto AddEmptyNode(T&& node) -> Node* {
    auto& new_node = nodesPool_.emplace_back(std::make_unique<Node>(std::forward<T>(node)));
    nodeProperties_.emplace(new_node.get(), NodeProperty{
      .Vertices = {},
      .NodeId = nextObjectId_++,
    });

    return new_node.get();
  }

  auto AddSpreadsheetNode() -> Node* {
    auto* node_ptr = AddEmptyNode(SpreadsheetNode{});
    MakeVertex("id", node_ptr, VertexDirection::Input);
    return node_ptr;
  }

  [[maybe_unused]] constexpr auto MakeVertex(std::string name, Node* node,
                                             VertexDirection direction) -> VertexType {
    const auto vtx_id = nextObjectId_++;

    auto vtx = boost::add_vertex(VertexProperty{
      .Name = std::move(name),
      .Node = node,
      .Direction = direction,
    }, graph_);

    nodeProperties_.at(node).Vertices.push_back(vtx);
    vertexIds_.emplace(vtx_id, vtx);

    return vtx;
  }

  [[maybe_unused]] constexpr auto MakeEdge(VertexType source, VertexType target) -> EdgeType {
    return boost::add_edge(source, target, EdgeProperty{.Id = nextObjectId_++}, graph_).first;
  }
};

} // namespace imsql::dg

#endif // IMSQL_MODELS_DG_DESIGN_GRAPH_MODEL_HPP
