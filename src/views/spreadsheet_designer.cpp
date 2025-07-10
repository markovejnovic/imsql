#include "imsqlite/math/interval.hpp"
#include "imsqlite/models/base_types.hpp"
#include "imsqlite/models/db.hpp"
#include "imsqlite/ui/base_types.hpp"
#include "imsqlite/ui/button.hpp"
#include "imsqlite/ui/nodes/editor.hpp"
#include "imsqlite/ui/nodes/input_attribute.hpp"
#include "imsqlite/ui/nodes/link.hpp"
#include "imsqlite/ui/nodes/node.hpp"
#include "imsqlite/ui/nodes/output_attribute.hpp"
#include "imsqlite/ui/nodes/static_attribute.hpp"
#include "imsqlite/ui/nodes/title_bar.hpp"
#include "imsqlite/ui/render_ctx.hpp"
#include "imsqlite/ui/text.hpp"
#include "imsqlite/views/spreadsheet_designer.hpp"
#include <boost/contract/assert.hpp>
#include <cstddef>
#include <cstdio>
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
constexpr auto kSpreadsheetColumnIdRange
  = math::MakeEndingInterval(kSpreadsheetNodeIdRange.Lower() + 8,
                             kSpreadsheetNodeIdRange.Upper());
constexpr math::Interval<int> kLinkNodeIdRange = kSpreadsheetNodeIdRange.Below(kAllotedLinkNodeIds);
constexpr math::Interval<int> kTableNodeIdRange = math::MakeStartingInterval(0, kAllotedTableIds);
constexpr math::Interval<int> kColumnNodeIdRange = kTableNodeIdRange.Above(kAllotedColumnNodeIds);

auto DbColumnId(int nodeId) -> models::ColumnId {
  BOOST_CONTRACT_ASSERT(kColumnNodeIdRange.Contains(nodeId));
  const auto col_id = gsl::narrow<std::size_t>(nodeId - kColumnNodeIdRange.Lower());
  BOOST_CONTRACT_ASSERT(col_id < kAllotedColumnNodeIds);
  return models::ColumnId{col_id};
}

auto SheetColumnId(int nodeId) -> models::ColumnId {
  BOOST_CONTRACT_ASSERT(kSpreadsheetColumnIdRange.Contains(nodeId));
  const auto col_id = gsl::narrow<std::size_t>(nodeId - kSpreadsheetColumnIdRange.Lower());
  BOOST_CONTRACT_ASSERT(col_id < kAllotedColumnNodeIds);
  return models::ColumnId{col_id};
}

auto DbNodeId(models::TableId tableId) -> int {
  const int node_id = gsl::narrow<int>(static_cast<std::size_t>(tableId));
  BOOST_CONTRACT_ASSERT(kTableNodeIdRange.Contains(node_id));
  return node_id;
}

auto DbNodeId(models::ColumnId colId) -> int {
  const int node_id = gsl::narrow<int>(static_cast<std::size_t>(colId)) +
                      kColumnNodeIdRange.Lower();
  BOOST_CONTRACT_ASSERT(kColumnNodeIdRange.Contains(node_id));
  return node_id;
}

auto MainSheetNodeId(models::ColumnId colId) -> int {
  const int node_id = gsl::narrow<int>(static_cast<std::size_t>(colId)) +
                      kSpreadsheetColumnIdRange.Lower();
  BOOST_CONTRACT_ASSERT(kSpreadsheetColumnIdRange.Contains(node_id));
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

/// @brief Renders the spreadsheet node, which is a unique node which allows the user to design the
///        spreadsheet layout.
/// @note Must be done within a nodes::Editor context.
void SpreadsheetNode(ui::RenderCtx& ctx) {
  auto& ctrls = ctx.Controllers();

  ui::nodes::Node spreadsheet_node{ctx, kSpreadsheetNodeIdRange.Lower()};

  if (!ctx.ViewStates.SpreadsheetDesigner.HasPainted) {
    // The first paint requires we place the nodes in the values stored in app-data.
    const boost::optional maybe_node_pos =
      ctrls.Db.CModel().CPersistentData().SpreadsheetNodePosition;
    if (maybe_node_pos.has_value()) {
      spreadsheet_node.SetPosition(*maybe_node_pos);
    } else {
      // If we don't have a position, we just place it at the origin.
      spreadsheet_node.SetPosition(ui::Vec2{0.0F, 0.0F});
    }
  }

  {
    ui::nodes::TitleBar title_bar{ctx, std::format("Spreadsheet ({})", spreadsheet_node.Id())};
  }

  // We need to go ahead and render each column defined in the spreadsheet.
  for (const auto& col_id : ctrls.Sheets.CModel().CColumns()) {
    const auto col_name = ctrls.Sheets.CModel().CColumnName(col_id);
    const auto col_node_id = MainSheetNodeId(col_id);

    {
      ui::nodes::InputAttribute input_attr{ctx, col_node_id};
      ui::Text col_name_text{ctx, std::format("{} ({})", col_name, col_node_id)};
    }
  }

  {
    ui::nodes::StaticAttribute static_attr{ctx, kSpreadsheetColumnIdRange.Upper() - 1};
    std::pair<ui::Button, bool> add_column_button
      = ui::Button::CreateAndIsClicked(ctx, "Add Column");
    if (add_column_button.second) {
      ctx.Controllers().Sheets.AddNewColumn();
    }
  }

  // Don't forget to store the persistent position of the spreadsheet node.
  ctx.Controllers().Db.UpdatePersistentData([&] (auto& pdata) {
    pdata.SpreadsheetNodePosition = spreadsheet_node.Position();
  });
}

} // namespace

void SpreadsheetDesigner(ui::RenderCtx& ctx) {
  {
    ui::nodes::Editor editor{ctx};

    const auto render_tables = [&ctx]() {
      for (const auto& [tbl_id, table_name] : ctx.Controllers().Db.CModel().Tables()) {
        const auto tbl_node_id = DbNodeId(tbl_id);

        ui::nodes::Node tbl_node{ctx, tbl_node_id};
        if (!ctx.ViewStates.SpreadsheetDesigner.HasPainted) {
          // The first paint requires we place the nodes in the values stored in app-data.
          if (ctx.Controllers().Db.CModel().CPersistentData().TableNodePositions.contains(tbl_id)) {
            tbl_node.SetPosition(
              ctx.Controllers().Db.CModel().CPersistentData().TableNodePositions.at(tbl_id));
          } else {
            // If we don't have a position, we just place it at the origin.
            tbl_node.SetPosition(ui::Vec2{0.0F, 0.0F});
          }
        }

        {
          ui::nodes::TitleBar title_bar{ctx, std::format("{} ({})", table_name, tbl_node_id)};
        }

        auto column_view = ctx.Controllers().Db.CModel().Columns(tbl_id)
          | std::views::transform([&](const models::ColumnId& col_id) {
              return std::make_pair(col_id, ctx.Controllers().Db.CModel().ColumnName(col_id));
            });

        for (const auto& [col_id, col_name] : column_view) {
          const auto col_node_id = DbNodeId(col_id);
          const auto col_string = std::format("{} ({})", col_name, col_node_id);

          switch (ctx.Controllers().Db.CModel().Relationship(col_id)) {
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
        ctx.Controllers().Db.UpdatePersistentData([&] (auto& pdata) {
          pdata.TableNodePositions[tbl_id] = tbl_node.Position();
        });
      }
    };

    const auto render_links = [&ctx]() {
      // All the database links.
      for (const auto& [rel_id, rel_edge] : ctx.Controllers().Db.CModel().Relationships()) {
        ui::nodes::Link link_node{ctx, LinkNodeId(rel_id),
                                  DbNodeId(rel_id.FromColumn),
                                  DbNodeId(rel_id.ToColumn)};
      }

      // No we need all the links between the database and the main sheet.
      for (const auto& rel : ctx.Controllers().CrossRelationships()) {
        ui::nodes::Link link_node{
          ctx, LinkNodeId(rel),
          DbNodeId(rel.FromColumn),
          MainSheetNodeId(rel.ToColumn),
          ui::nodes::LinkColorStyle {
            .Normal = ctx.Controllers().Theme.Model().UserDrawnLinkColor,
            .Selected = ctx.Controllers().Theme.Model().UserDrawnLinkColorSelected,
            .Hovered = ctx.Controllers().Theme.Model().UserDrawnLinkColorHover,
          },
        };
      }
    };

    render_tables();
    render_links();
    SpreadsheetNode(ctx);
  }

  // Let's handle all the links now.
  auto maybe_link = ui::nodes::Link::WhichLinkCreated();
  if (maybe_link.has_value()) {
    const auto& [start_attr_id, end_attr_id] = *maybe_link;
    // TODO(marko): This if-else feels like a pretty ill hack, but idk
    if (kSpreadsheetColumnIdRange.Contains(end_attr_id)) {
      ctx.Controllers().LinkBetweenDbAndMainSheet(DbColumnId(start_attr_id),
                                                  SheetColumnId(end_attr_id));
    } else {
      ctx.Controllers().LinkWithinDb(DbColumnId(start_attr_id),
                                     DbColumnId(end_attr_id));
    }
  }

  ctx.ViewStates.SpreadsheetDesigner.HasPainted = true;
}

} // namespace imsql::views
