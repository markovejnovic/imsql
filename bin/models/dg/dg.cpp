#include "models/db_model.hpp"
#include "util/views/dedup.hpp"
#include "util/views/join_with.hpp"
#include "models/dg/db_table_node.hpp"
#include "models/dg/dg.hpp"
#include "pch.hpp"
#include <boost/graph/graphviz.hpp>

namespace imsql::dg {

DesignGraphModel::DesignGraphModel(DbModel* dbModel)
    : dbModel_(dbModel) {
  const auto attach_db = [this](){
    // TODO(marko): If I knew my algorithms, perhaps we would be able to avoid this map.
    std::unordered_map<const DbColumnModel*, VertexType> scratchpad;

    // First we need to insert all the tables, before we even consider relationships.
    for (auto& table : dbModel_->Tables()) {
      auto* table_node_ptr = AddEmptyNode(std::make_unique<DbTableNode>(&table));

      for (const auto& column : table.Columns()) {
        scratchpad[&column] = MakeVertex(column.Name(), table_node_ptr, VertexDirection::Output);
      }
    }

    // Okay, now it's time to add the relationships for the edges.
    for (const auto& [from, to] : dbModel_->Relationships()) {
      const auto from_vtx = scratchpad[from];
      const auto to_vtx = scratchpad[to];
      // Note that instead of labeling the to vertex as output, we label it as bidirectional. This
      // model is important as it allows us to propagate information from upstream.
      graph_[to_vtx].Direction = VertexDirection::Bidirectional;
      MakeEdge(from_vtx, to_vtx);
    }
  };

  attach_db();
}

auto DesignGraphModel::GetId(const Node* node) const noexcept -> int {
  return nodeProperties_.at(node).NodeId;
}

auto DesignGraphModel::GetId(const EdgeType& edge) const noexcept -> int {
  return graph_[edge].Id;
};

void DesignGraphModel::DumpDot(std::ostream& out) const {
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

} // namespace imsql::dg
