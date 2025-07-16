#include "models/db_model.hpp"
#include "models/dg/nodes/db_table_node.hpp"
#include "util/views/dedup.hpp"
#include "util/views/join_with.hpp"
#include "models/dg/dg.hpp"
#include "pch.hpp"
#include "util/fp.hpp"
#include <boost/graph/graphviz.hpp>

namespace imsql::dg {

DesignGraphModel::DesignGraphModel(DbModel* dbModel)
    : dbModel_(dbModel),
      // @todo(marko): I would like to be able to support multiple spreadsheets, but for now this
      // will suffice.
      spreadsheetNode_(AddSpreadsheetNode()) {
  const auto attach_db = [this](){
    // TODO(marko): If I knew my algorithms, perhaps we would be able to avoid this map.
    std::unordered_map<const DbColumnModel*, VertexType> scratchpad;

    // First we need to insert all the tables, before we even consider relationships.
    for (auto& table : dbModel_->Tables()) {
      // @todo(marko): This may not need to be a unique pointer considering that we are not using
      // polymorphism here.
      auto* table_node_ptr = AddEmptyNode(DbTableNode{this, &*table});

      for (const auto& column : table->Columns()) {
        auto column_vtx = std::make_shared<DbColumnVertex>(this, &*column);
        table_node_ptr->AddVertex(column_vtx.get());
        scratchpad[column.get()] = MakeVertex(column_vtx, VertexDirection::Output,
                                              table_node_ptr);
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

auto DesignGraphModel::GetRowIds() const noexcept -> std::vector<std::size_t> {
  // We need to get the row IDs from the spreadsheet node.
  if (spreadsheetNode_ == nullptr) {
    return {};
  }

  // Now we need to find the id vertex of the spreadsheet node.
  // @todo(marko): This implementation obviously sucks.
  const auto& vertices = Vertices(spreadsheetNode_);
  auto id_vertex = *std::ranges::find_if(
    vertices,
    [&](const VertexType& vtx) {
      return graph_[vtx].Direction == VertexDirection::Input &&
             graph_[vtx].Vertex->Name() == "id";
    }
  );

  BaseVertex& id_vtx_ptr = *graph_[id_vertex].Vertex;
  auto id_values = id_vtx_ptr.GetAllValues();

  return std::ranges::to<std::vector<std::size_t>>(
    id_values
      | std::views::transform([](const Value& val) {
        if (std::holds_alternative<Int64Value>(val)) {
          return std::get<Int64Value>(val).Value();
        }
        throw std::runtime_error("Expected Int64Value for row ID.");
      })
  );
}

} // namespace imsql::dg
