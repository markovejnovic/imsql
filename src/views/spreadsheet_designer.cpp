#include "imsqlite/math/interval.hpp"
#include "imsqlite/models/base_types.hpp"
#include "imsqlite/models/db.hpp"
#include "imsqlite/ui/base_types.hpp"
#include "imsqlite/ui/nodes/editor.hpp"
#include "imsqlite/ui/nodes/input_attribute.hpp"
#include "imsqlite/ui/nodes/link.hpp"
#include "imsqlite/ui/nodes/node.hpp"
#include "imsqlite/ui/nodes/output_attribute.hpp"
#include "imsqlite/ui/nodes/title_bar.hpp"
#include "imsqlite/ui/render_ctx.hpp"
#include "imsqlite/ui/text.hpp"
#include "imsqlite/views/spreadsheet_designer.hpp"
#include <boost/contract/assert.hpp>
#include <gsl/gsl>
#include <limits>
#include <ranges>
#include <bit>

namespace imsql::views {

namespace {

constexpr int kAllotedTableIds = 4096;
constexpr std::size_t kAllotedColumnNodeIds = 8096;
// Note if you change kAllotedLinkNodeIds you risk breaking LinkNodeId. See the implementation
// there to understand why.
constexpr std::size_t kAllotedLinkNodeIds = 2UL << (std::bit_width(kAllotedColumnNodeIds) * 2UL);

constexpr auto kSpreadsheetNodeIdRange = math::MakeEndingInterval(std::numeric_limits<int>::max(),
                                                                  1024);
constexpr math::Interval<int> kLinkNodeIdRange = kSpreadsheetNodeIdRange.Below(kAllotedLinkNodeIds);
constexpr math::Interval<int> kTableNodeIdRange = math::MakeStartingInterval(0, kAllotedTableIds);
constexpr math::Interval<int> kColumnNodeIdRange = kTableNodeIdRange.Above(kAllotedColumnNodeIds);

auto TableNodeId(models::TableId tableId) -> int {
  const int node_id = gsl::narrow<int>(static_cast<std::size_t>(tableId));
  BOOST_CONTRACT_ASSERT(kTableNodeIdRange.Contains(node_id));
  return node_id;
}

auto ColumNodeId(models::ColumnId colId) -> int {
  const int node_id = gsl::narrow<int>(static_cast<std::size_t>(colId)) +
                      kColumnNodeIdRange.Lower();
  BOOST_CONTRACT_ASSERT(kColumnNodeIdRange.Contains(node_id));
  return node_id;
}

auto LinkNodeId(models::RelationshipEdge relEdge) -> int {
  const auto from_id = static_cast<std::size_t>(relEdge.FromColumn);
  BOOST_CONTRACT_ASSERT(from_id <= kColumnNodeIdRange.Length());

  const auto to_id = static_cast<std::size_t>(relEdge.ToColumn);
  BOOST_CONTRACT_ASSERT(to_id <= kColumnNodeIdRange.Length());

  // Now we create a new integer which will fit the two integers like so:
  // x1, x2 -> y
  //
  // | x1 | x2 |
  // |    y    |
  const unsigned width = std::bit_width(static_cast<unsigned int>(kColumnNodeIdRange.Length()));
  const auto from_to_id = to_id << width | from_id;

  const int node_id = gsl::narrow<int>(from_to_id) + kLinkNodeIdRange.Lower();
  BOOST_CONTRACT_ASSERT(kLinkNodeIdRange.Contains(node_id));
  return node_id;
}

} // namespace

void SpreadsheetDesigner(ui::RenderCtx& ctx, models::AppState& appState) {
  ui::nodes::Editor editor{ctx};

  const auto render_tables = [&ctx, &appState]() {
    for (const auto& [table_id, table_name] : appState.Database.Tables()) {
      const auto tbl_node_id = TableNodeId(table_id);

      ui::nodes::Node tbl_node{ctx, tbl_node_id};
      if (!appState.ViewStates.SpreadsheetDesigner.HasPainted) {
        // The first paint requires we place the nodes in the values stored in app-data.
        if (appState.Database.PersistentData().TableNodePositions.contains(table_id)) {
          tbl_node.SetPosition(appState.Database.PersistentData().TableNodePositions.at(table_id));
        } else {
          // If we don't have a position, we just place it at the origin.
          tbl_node.SetPosition(ui::Vec2{0.0F, 0.0F});
        }
      }

      {
        ui::nodes::TitleBar title_bar{ctx, std::format("{} ({})", table_name, tbl_node_id)};
      }

      auto column_view = appState.Database.Columns(table_id)
        | std::views::transform([&](const models::ColumnId& col_id) {
            return std::make_pair(col_id, appState.Database.ColumnName(col_id));
          });

      for (const auto& [col_id, col_name] : column_view) {
        const auto col_node_id = ColumNodeId(col_id);
        const auto col_string = std::format("{} ({})", col_name, col_node_id);

        switch (appState.Database.Relationship(col_id)) {
          case models::Db::RelationshipType::LeftToRight: {
            ui::nodes::InputAttribute input_attr{ctx, col_node_id};
            ui::Text col_name_text{ctx, col_string};
            break;
          }
          case models::Db::RelationshipType::None:
          case models::Db::RelationshipType::RightToLeft: {
            ui::nodes::OutputAttribute output_attr{ctx, col_node_id};
            ui::Text col_name_text{ctx, col_string};
            break;
          }
        }
      }
      appState.Database.PersistentData().TableNodePositions[table_id] = tbl_node.Position();
    }
  };

  const auto render_links = [&appState, &ctx]() {
    for (const auto& [rel_id, rel_edge] : appState.Database.Relationships()) {
      ui::nodes::Link link_node{ctx, LinkNodeId(rel_id),
                                ColumNodeId(rel_id.FromColumn),
                                ColumNodeId(rel_id.ToColumn)};
    }
  };

  render_tables();
  render_links();

  appState.ViewStates.SpreadsheetDesigner.HasPainted = true;
}

} // namespace imsql::views
