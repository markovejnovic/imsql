#include "imsqlite/dsalg/views/dedup.hpp"
#include "imsqlite/dsalg/views/join_with.hpp"
#include "imsqlite/models/dg/db_table_node.hpp"
#include "imsqlite/models/dg/dg.hpp"
#include "imsqlite/pch/std.hpp"
#include <boost/graph/graphviz.hpp>

namespace imsql::models::dg {

void DG::AttachDatabase(const models::Db& database) {
  // TODO(marko): If I knew my algorithms, perhaps we would be able to avoid this map.
  boost::unordered_map<ColumnId, VertexType> scratchpad;

  // First we need to insert all the tables, before we even consider relationships.
  for (const auto& [table_id, table_name] : database.Tables()) {
    auto* table_node_ptr =
      AddEmptyNode(std::make_unique<DbTableNode>(database, table_id));

    // Let's add vertices for each column.
    for (const auto& column_id : database.Columns(table_id)) {
      scratchpad[column_id] = MakeVertex(
        database.ColumnName(column_id),
        table_node_ptr,
        VertexNodeDirection::Output);
    }
  }

  // Okay, now it's time to add the relationships for the edges.
  for (const auto& [edge, enforcement] : database.Relationships()) {
    // @todo Investigate why ToColumn is referring to the foreign key.
    const auto from_vtx = scratchpad[edge.ToColumn];
    const auto to_vtx = scratchpad[edge.FromColumn];
    // We need to patch the vertex directions.
    graph_[from_vtx].Direction = VertexNodeDirection::Output;
    graph_[to_vtx].Direction = VertexNodeDirection::Input;
    MakeEdge(from_vtx, to_vtx);
  }
}

auto DG::GetId(const Node* node) const noexcept -> int {
  return nodeProperties_.at(node).NodeId;
}

auto DG::GetId(const VertexType& vertex) const noexcept -> int {
  return graph_[vertex].Id;
}

auto DG::GetId(const EdgeType& edge) const noexcept -> int {
  return graph_[edge].Id;
};

void DG::DumpDot(std::ostream& out) const {
  // Before we output anything, let's transform the graph vertices pointing to nodes, to the
  // inverse -- nodes pointing to vertices.
  std::multimap<Node*, VertexType> node_to_vertex_map;

  for (auto [v, vend] = vertices(graph_); v != vend; ++v) {
    const auto& prop = graph_[*v];
    node_to_vertex_map.emplace(prop.Node, *v);
  }

  out << "digraph G {\n";

  // For each node, we want to output it.
  auto dot_view = node_to_vertex_map
    | std::views::keys
    | imsql::vw::dedup
    | std::views::transform([&](Node* node) {
        const auto [fst, snd] = node_to_vertex_map.equal_range(node);
        auto vertices_range = std::ranges::subrange(fst, snd);

        auto vertices_str_view = vertices_range
          | std::views::transform([](const auto& pair) { return pair.second; })
          | std::views::transform([&](const auto& vtx) {
            return std::format("<{}> {}", graph_[vtx].Name, graph_[vtx].Name);
          })
          | imsql::vw::intersperse(std::string(" | "));

        return std::format(
          R"("{}" [label="{{{} | {}}}" shape="record"])",
          node->Name(), node->Name(),
          std::ranges::fold_left(
            vertices_str_view, std::string(),
            [](const std::string& acc, const std::string& vtx_name) {
              return acc + vtx_name;
            })
          );
      });

  // Render all the nodes.
  for (const auto& line : dot_view) {
    out << line << "\n";
  }

  // Render all the edges.
  auto [ei, ei_end] = boost::edges(graph_);
  for (; ei != ei_end; ++ei) {
      auto source_v = boost::source(*ei, graph_);
      auto target_v = boost::target(*ei, graph_);

      const auto& source_prop = graph_[source_v];
      const auto& target_prop = graph_[target_v];

      const auto& source_node_name = source_prop.Node->Name();
      const auto& target_node_name = target_prop.Node->Name();

      // Output the edge in DOT format.
      out << std::format("{} -> {}",
                         std::format("{}:{}", source_node_name, source_prop.Name),
                         std::format("{}:{}", target_node_name, target_prop.Name))
          << ";\n";
  }

  out << "}\n";
}

} // namespace imsql::models::dg
