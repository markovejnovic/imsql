#ifndef IMSQL_MODELS_DG_GRAPH_TYPES_HPP
#define IMSQL_MODELS_DG_GRAPH_TYPES_HPP

namespace imsql::dg {

class BaseVertex;

enum class VertexDirection : uint8_t {
  Input,
  Output,
  Bidirectional,
};

struct VertexProperty {
  /// @todo(marko): This could be a unique_ptr but boost::add_vertex requires a copyable type. This
  /// is an escape hatch for now, but we should consider using a unique_ptr in the future.
  std::shared_ptr<BaseVertex> Vertex;
  VertexDirection Direction;
};

struct EdgeProperty {
  int Id;
};

using GraphType = boost::adjacency_list<
  boost::vecS, boost::vecS, boost::bidirectionalS,
  VertexProperty, EdgeProperty
>;

using VertexType = boost::graph_traits<GraphType>::vertex_descriptor;

static_assert(std::is_same_v<VertexType, unsigned long>,
              "VertexType must be the same as VertexDescriptor.");

using EdgeType = boost::graph_traits<GraphType>::edge_descriptor;

struct NodeProperty {
  std::vector<VertexType> Vertices;
  int NodeId;
};

} // namespace imsql::dg

#endif // IMSQL_MODELS_DG_GRAPH_TYPES_HPP
