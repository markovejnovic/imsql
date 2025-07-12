#ifndef IMSQLITE_MODELS_DG_DG_HPP
#define IMSQLITE_MODELS_DG_DG_HPP

#include "imsqlite/models/dg/node.hpp"
#include "imsqlite/models/dg/operator_node.hpp"
#include "imsqlite/pch/std.hpp"
#include "imsqlite/pch/graph.hpp"
#include "imsqlite/models/db.hpp"

namespace imsql::models::dg {

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
class DG {
public:
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
  std::vector<std::unique_ptr<Node>> nodesPool_;

  /// @brief Maps nodes to their properties.
  boost::unordered_map<const Node*, NodeProperty> nodeProperties_;
  boost::unordered_map<int, VertexType> vertexIds_;

  int nextObjectId_ = 0;

public:
  /// @brief Register a database table node in the design graph.
  void AttachDatabase(const models::Db& database);

  /// @brief Output the design graph in DOT format, useful for debugging.
  void DumpDot(std::ostream& out) const;

  [[nodiscard]] constexpr auto Nodes() const noexcept {
    return nodesPool_ | std::views::transform([](const auto& node) { return node.get(); });
  }

  [[nodiscard]] constexpr auto Vertices(const Node* node) const noexcept {
    return nodeProperties_.at(node).Vertices;
  }

  [[nodiscard]] constexpr auto GetVertexDirection(VertexType vtx) const noexcept {
    return graph_[vtx].Direction;
  }

  [[nodiscard]] constexpr auto VertexName(VertexType vtx) const noexcept {
    return graph_[vtx].Name;
  }

  [[nodiscard]] constexpr auto VertexNode(VertexType vtx) const noexcept {
    return graph_[vtx].Node;
  }

  [[nodiscard]] constexpr auto VertexCount() const noexcept {
    return boost::num_vertices(graph_);
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

  /// @brief Retrieve the unique identifier for the given object.
  /// @{
  [[nodiscard]] auto GetId(const Node* node) const noexcept -> int;
  [[nodiscard]] auto GetId(const EdgeType& edge) const noexcept -> int;
  /// @}

  template <class T>
  void AddOperatorNode() {
    static_assert(std::is_base_of_v<OperatorNode, T>, "T must be derived from OperatorNode.");
    auto* node_ptr = AddEmptyNode(std::make_unique<T>());
    // @todo Please make the vertex adding shared with the DB behavior in the cpp
    for (const auto& input_field : node_ptr->InputFields()) {
      MakeVertex(std::string(input_field), node_ptr, VertexDirection::Input);
    }

    for (const auto& output_field : node_ptr->OutputFields()) {
      MakeVertex(std::string(output_field), node_ptr, VertexDirection::Output);
    }
  }

  constexpr void AddEdge(VertexType source, VertexType target) {
    MakeEdge(source, target);
  }

private:
  /// @brief Register a node with the system which has no vertices associated with it.
  template <class T>
  constexpr auto AddEmptyNode(std::unique_ptr<T>&& node) -> T* {
    static_assert(std::is_base_of_v<Node, T>, "T must be derived from Node.");

    auto* node_ptr = node.get();
    nodesPool_.emplace_back(std::move(node));
    nodeProperties_.emplace(node_ptr, NodeProperty{
      .Vertices = {},
      .NodeId = nextObjectId_++,
    });

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

} // namespace imsql::models::dg

#endif // IMSQLITE_MODELS_DG_DG_HPP
