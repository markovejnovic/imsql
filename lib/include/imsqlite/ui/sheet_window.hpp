#ifndef IMSQLITE_UI_SHEET_WINDOW_HPP
#define IMSQLITE_UI_SHEET_WINDOW_HPP

#include "imgui.h"
#include "imnodes.h"
#include "imsqlite/db/db.hpp"
#include "imsqlite/deprecated.hpp"
#include "imsqlite/ui/style.hpp"
#include "imsqlite/_model/sheet.hpp"
#include <boost/contract/assert.hpp>
#include <cstdio>
#include <gsl/narrow>
#include <limits>
#include <gsl/gsl>
#include <optional>
#include <print>
#include <ranges>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

namespace imsql::ui {

inline void SpreadsheetTab(const model::Sheet& sheet) {
  [[maybe_unused]] auto style = MakeStyle(
    ImGuiStyleVar_CellPadding, ImVec2(0.0F, 0.0F),
    ImGuiStyleVar_FramePadding, ImVec2(0.0F, 0.0F)
  );

  static constexpr ImGuiTableFlags spreadsheet_flags = (
    static_cast<unsigned int>(ImGuiTableFlags_Borders) |
    static_cast<unsigned int>(ImGuiTableFlags_RowBg) |
    static_cast<unsigned int>(ImGuiTableFlags_ScrollY) |
    static_cast<unsigned int>(ImGuiTableFlags_Resizable)
  );

  if (ImGui::BeginTable("main_view",
                        gsl::narrow<int>(sheet.CSpreadSheet().CColumns().size()),
                        spreadsheet_flags)) {
    // Render the header
    for (const auto& column : sheet.CSpreadSheet().CColumns()) {
      ImGui::TableSetupColumn(column.CName().c_str(),
                              ImGuiTableColumnFlags_WidthStretch);
    }
    ImGui::TableHeadersRow();

    // Render the rows
    for (const auto& row : sheet.Rows()) {
      ImGui::TableNextRow();

      const auto& cells = row.Cells();
      for (size_t col_idx = 0; col_idx < cells.size(); ++col_idx) {
        const auto& cell_variant = cells[col_idx];
        ImGui::TableSetColumnIndex(static_cast<int>(col_idx));
        std::visit([](const auto& cell) {
          ImGui::TextUnformatted(cell.AsString().c_str());
        }, cell_variant);
      }
    }

    // Before we wrap up, we need to add an additional fake row which will
    // allow the user to add new rows.
    ImGui::TableNextRow();

    ImGui::EndTable();
  }
}

/// @return Whether the user requested a new column be added.
template <bool IsFirstPaint>
auto DesignerTab(
  const DesignerTabState<IsFirstPaint>& designerState,
  const model::Sheet& sheet
) -> std::pair<DesignerTabState<false>, model::Sheet> {
  model::Sheet new_sheet = sheet;
  DesignerTabState new_designer_state = designerState;

  static constexpr std::pair<int, int> kSpreadsheetNodeIdRange = {
    std::numeric_limits<int>::max() - 1024,
    std::numeric_limits<int>::max() - 1,
  };
  static constexpr std::pair<int, int> kSpreasheetAttrIdRange = {
    kSpreadsheetNodeIdRange.first,
    kSpreadsheetNodeIdRange.second - 16,
  };
  static constexpr int kOutputNodeId = kSpreadsheetNodeIdRange.second;
  static constexpr int kNewColumnAttributeId =
    kSpreadsheetNodeIdRange.second - 1;

  // TODO(marko): This is kind of hacked together. I would prefer if
  // Spreadsheet were part of the same generator which generates unique IDs for
  // each object in the application, just like Db.
  const auto spreadsheetColToAttr = [](std::size_t idx) -> int {
    // This is a hack to ensure that the spreadsheet columns are always
    // assigned IDs in the range of [0, 1023].
    const int attr_idx = gsl::narrow<int>(idx + kSpreasheetAttrIdRange.first);
    BOOST_CONTRACT_ASSERT(attr_idx < kSpreasheetAttrIdRange.second);
    return attr_idx;
  };

  const auto spreadsheetAttrToCol = [](int attrId) -> std::size_t {
    BOOST_CONTRACT_ASSERT(
      attrId >= kSpreasheetAttrIdRange.first
      && attrId < kSpreasheetAttrIdRange.second
    );
    return gsl::narrow<std::size_t>(attrId - kSpreasheetAttrIdRange.first);
  };

  const auto MakeOutputNode = [&]() -> std::vector<int> {
    static constexpr unsigned int kSpreadsheetNodeColor =
      IM_COL32(29, 111, 66, 255);

    {
      auto node_colors = MakeNodeColorStyle(
        ImNodesCol_TitleBar, kSpreadsheetNodeColor,
        ImNodesCol_TitleBarHovered, kSpreadsheetNodeColor,
        ImNodesCol_TitleBarSelected, kSpreadsheetNodeColor
      );

      Node spreadsheet_node{kOutputNodeId};
      if (!new_designer_state.HasPainted
          && new_designer_state.TableNodePositions.contains(kOutputNodeId)) {
        // If we are painting for the first time, and have a saved position, we
        // want to set the position of the node to that saved position.
        spreadsheet_node.SetPosition(
          new_designer_state.TableNodePositions[kOutputNodeId]);
      }

      const auto style = MakeStyle(
        ImGuiStyleVar_FramePadding, ImVec2(0.0F, 0.0F),
        ImGuiStyleVar_CellPadding, ImVec2(0.0F, 0.0F)
      );
      {
        NodeTitleBar title_bar{"Spreadsheet"};
      }

      for (std::size_t i = 0; i < sheet.CSpreadSheet().CColumns().size(); ++i) {
        const auto& column = sheet.CSpreadSheet().CColumns()[i];
        const int attr_idx = spreadsheetColToAttr(i);

        NodeInputAttribute input_attr{attr_idx};
        InputText("", new_sheet.Spreadsheet().Columns()[i].Name());
      }

      {
        NodeStaticAttribute new_column_attr{kNewColumnAttributeId};
        if (ImGui::Button("Add New Column")) {
          new_sheet.Spreadsheet().NewColumn();
        }
      }

      new_designer_state.TableNodePositions[kOutputNodeId] =
        spreadsheet_node.Position();

      return std::views::iota(0ULL, sheet.CSpreadSheet().CColumns().size())
        | std::views::transform(spreadsheetColToAttr)
        | std::ranges::to<std::vector<int>>();
    }
  };

  std::vector<int> output_attributes;

  {
    NodeEditor node_editor;

    // Add all the tables
    for (const auto& [table_id, table] : sheet.CDb().Tables()) {
      const auto node_id = gsl::narrow<int>(table_id);
      Node table_node{node_id};

      if (!new_designer_state.HasPainted
          && new_designer_state.TableNodePositions.contains(node_id)) {
        // If we are painting for the first time, and have a saved position, we
        // want to set the position of the node to that saved position.
        table_node.SetPosition(new_designer_state.TableNodePositions[node_id]);
      }

      {
        NodeTitleBar title_bar{
          std::format("{} ({})", table.Name, table_id).c_str(),
        };
      }

      for (const auto& col_id : table.Columns) {
        const Column& column = sheet.CDb().ColumnInfo(col_id);

        const int col_node_id = gsl::narrow<int>(col_id);

        switch (column.RelationshipType) {
          case RelationshipType::InboundRelationship: {
            NodeInputAttribute static_attr{col_node_id};
            ImGui::TextUnformatted(sheet.CDb().ColumnInfo(col_id).Name.c_str());
            break;
          }
          // We render no-relationships as output attributes. The reason being
          // is that this enables the user to create new relationships to the
          // main sheet as they see fit.
          case RelationshipType::NoRelationship:
          case RelationshipType::OutboundRelationship: {
            NodeOutputAttribute static_attr{col_node_id};
            ImGui::TextUnformatted(sheet.CDb().ColumnInfo(col_id).Name.c_str());
            break;
          }
        }
      }

      // After we're done painting these nodes, let's store them in the UI
      // state. Note we don't check if this is the first paint since we want to
      // track the total user position of each node.
      new_designer_state.TableNodePositions[node_id] =
        table_node.Position();
    }

    int link_id = 0;
    for (const auto& [rel_edge, rel] : sheet.CDb().Relationships()) {
       ImNodes::Link(link_id++,
                     gsl::narrow<int>(rel_edge.FromColumn),
                     gsl::narrow<int>(rel_edge.ToColumn));
    }

    output_attributes = MakeOutputNode();
  }

  // Now, let us test for link creation.
  int start_attr_id = 0;
  int end_attr_id = 0;
  if (ImNodes::IsLinkCreated(&start_attr_id, &end_attr_id)) {
    if (new_sheet.CDb().CColumns().contains(start_attr_id)) {
      // The first link belongs to a real column, cool.
      if (std::ranges::find(output_attributes, start_attr_id)
          != output_attributes.end()) {
        // The second link belongs to the spreadsheet output node, which is
        // perfectly legal, let's add a link!
      }
    } else if (new_sheet.CDb().CColumns().contains(end_attr_id)) {
      // The second link belongs to a real column, cool.
    } else {
      // TODO(marko): Implement the case of connecting unknown links.
    }
  }

  new_designer_state.HasPainted = true;
  return { new_designer_state, new_sheet };
}

template <bool IsFirstPaint>
auto SheetWindow(
  const UiState<IsFirstPaint>& uiState,
  const model::Sheet& sheet
) -> std::pair<UiState<false>, model::Sheet> {
  UiState<false> new_ui_state = {
    // TODO(marko): This analysis and subsequent reduction of the state is
    // bug-prone.
    .DesignerTabState = {
      .HasPainted = uiState.DesignerTabState.HasPainted,
      .TableNodePositions = uiState.DesignerTabState.TableNodePositions
    },
  };
  model::Sheet new_sheet = sheet;

  Window sheet_window{sheet.Title().c_str()};
  if (!sheet_window.Rendering()) {
    return std::make_pair(new_ui_state, sheet);
  }

  {
    TabBar tab_bar{std::format("{}_tabs", sheet.Title()).c_str()};
    if (!tab_bar.Rendering()) {
      return std::make_pair(new_ui_state, new_sheet);
    }

    {
      TabItem spreadsheet_tab{"Spreadsheet"};
      if (spreadsheet_tab.Rendering()) {
        SpreadsheetTab(sheet);
      }
    }

    {
      TabItem designer_tab{"Designer"};
      if (designer_tab.Rendering()) {
        std::tie(new_ui_state.DesignerTabState, new_sheet) =
          DesignerTab(new_ui_state.DesignerTabState, sheet);
      }
    }
  }

  return std::make_pair(
    UiState<false>{ .DesignerTabState = new_ui_state.DesignerTabState, },
    new_sheet
  );
}

} // namespace imsql::ui

#endif // IMSQLITE_UI_SHEET_WINDOW_HPP
