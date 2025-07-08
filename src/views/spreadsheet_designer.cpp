#include "imsqlite/views/spreadsheet_designer.hpp"
#include "imsqlite/ui/base_types.hpp"
#include "imsqlite/ui/nodes/editor.hpp"
#include "imsqlite/ui/nodes/input_attribute.hpp"
#include "imsqlite/ui/nodes/node.hpp"
#include "imsqlite/ui/nodes/static_attribute.hpp"
#include "imsqlite/ui/nodes/title_bar.hpp"
#include "imsqlite/ui/render_ctx.hpp"
#include "imsqlite/ui/text.hpp"
#include <gsl/gsl>
#include <ranges>

namespace imsql::views {

void SpreadsheetDesigner(ui::RenderCtx& ctx, models::AppState& appState) {
  ui::nodes::Editor editor{ctx};

  for (const auto& [table_id, table_name] : appState.Database.Tables()) {
    const auto tbl_node_id = gsl::narrow<int>(static_cast<std::size_t>(table_id));

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
      const auto col_node_id = gsl::narrow<int>(static_cast<std::size_t>(col_id));
      const auto col_string = std::format("{} ({})", col_name, col_node_id);

      if (appState.Database.HasRelationships(col_id)) {
        ui::nodes::InputAttribute input_attr{ ctx, col_node_id };
        ui::Text col_name{ctx, col_string};
      } else {
        ui::nodes::StaticAttribute static_attr{ctx, col_node_id};
        ui::Text col_name{ctx, col_string};
      }
    }

    // Whatever happens, we need to track the new position of the node.
    appState.Database.PersistentData().TableNodePositions[table_id] = tbl_node.Position();
  }

  appState.ViewStates.SpreadsheetDesigner.HasPainted = true;
}

} // namespace imsql::views
