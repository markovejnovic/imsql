#ifndef IMMM_TABLE_HPP
#define IMMM_TABLE_HPP

#include "imgui.h"
#include "immm/component.hpp"
#include "immm/detail/pch.hpp"
#include "immm/render_ctx.hpp"

namespace immm {

enum class TableFlag {
  kNone = ImGuiTableFlags_None,
  kResizable = ImGuiTableFlags_Resizable,
  kReorderable = ImGuiTableFlags_Reorderable,
  kHideable = ImGuiTableFlags_Hideable,
  kSortable = ImGuiTableFlags_Sortable,
  kNoSavedSettings = ImGuiTableFlags_NoSavedSettings,
  kContextMenuInBody = ImGuiTableFlags_ContextMenuInBody,
  kRowBg = ImGuiTableFlags_RowBg,
  kBordersInnerH = ImGuiTableFlags_BordersInnerH,
  kBordersOuterH = ImGuiTableFlags_BordersOuterH,
  kBordersInnerV = ImGuiTableFlags_BordersInnerV,
  kBordersOuterV = ImGuiTableFlags_BordersOuterV,
  kBordersH = ImGuiTableFlags_BordersH,
  kBordersV = ImGuiTableFlags_BordersV,
  kBordersInner = ImGuiTableFlags_BordersInner,
  kBordersOuter = ImGuiTableFlags_BordersOuter,
  kBorders = ImGuiTableFlags_Borders,
  kNoBordersInBody = ImGuiTableFlags_NoBordersInBody,
  kNoBordersInBodyUntilResize = ImGuiTableFlags_NoBordersInBodyUntilResize,
  kSizingFixedFit = ImGuiTableFlags_SizingFixedFit,
  kSizingFixedSame = ImGuiTableFlags_SizingFixedSame,
  kSizingStretchProp = ImGuiTableFlags_SizingStretchProp,
  kSizingStretchSame = ImGuiTableFlags_SizingStretchSame,
  kNoHostExtendX = ImGuiTableFlags_NoHostExtendX,
  kNoHostExtendY = ImGuiTableFlags_NoHostExtendY,
  kNoKeepColumnsVisible = ImGuiTableFlags_NoKeepColumnsVisible,
  kPreciseWidths = ImGuiTableFlags_PreciseWidths,
  kNoClip = ImGuiTableFlags_NoClip,
  kPadOuterX = ImGuiTableFlags_PadOuterX,
  kNoPadOuterX = ImGuiTableFlags_NoPadOuterX,
  kNoPadInnerX = ImGuiTableFlags_NoPadInnerX, 
  kScrollX = ImGuiTableFlags_ScrollX,
  kScrollY = ImGuiTableFlags_ScrollY,
  kSortMulti = ImGuiTableFlags_SortMulti,
  kSortTristate = ImGuiTableFlags_SortTristate,
  kHighlightHoveredColumn = ImGuiTableFlags_HighlightHoveredColumn,
};

[[nodiscard]] constexpr auto operator|(TableFlag lhs, TableFlag rhs) -> TableFlag {
  return static_cast<TableFlag>(static_cast<ImGuiTableFlags>(lhs)
                              | static_cast<ImGuiTableFlags>(rhs));
}

enum class TableColumnFlag {
  kNone = ImGuiTableColumnFlags_None,
  kDisabled = ImGuiTableColumnFlags_Disabled,
  kDefaultHide = ImGuiTableColumnFlags_DefaultHide,
  kDefaultSort = ImGuiTableColumnFlags_DefaultSort,
  kWidthStretch = ImGuiTableColumnFlags_WidthStretch,
  kWidthFixed = ImGuiTableColumnFlags_WidthFixed,
  kNoResize = ImGuiTableColumnFlags_NoResize,
  kNoReorder = ImGuiTableColumnFlags_NoReorder,
  kNoHide = ImGuiTableColumnFlags_NoHide,
  kNoClip = ImGuiTableColumnFlags_NoClip,
  kNoSort = ImGuiTableColumnFlags_NoSort,
  kNoSortAscending = ImGuiTableColumnFlags_NoSortAscending,
  kNoSortDescending = ImGuiTableColumnFlags_NoSortDescending,
  kNoHeaderLabel = ImGuiTableColumnFlags_NoHeaderLabel,
  kNoHeaderWidth = ImGuiTableColumnFlags_NoHeaderWidth,
  kPreferSortAscending = ImGuiTableColumnFlags_PreferSortAscending,
  kPreferSortDescending = ImGuiTableColumnFlags_PreferSortDescending,
  kIndentEnable = ImGuiTableColumnFlags_IndentEnable,
  kIndentDisable = ImGuiTableColumnFlags_IndentDisable,
  kAngledHeader = ImGuiTableColumnFlags_AngledHeader,
};

[[nodiscard]] constexpr auto operator|(TableColumnFlag lhs,
                                       TableColumnFlag rhs) -> TableColumnFlag {
  return static_cast<TableColumnFlag>(static_cast<ImGuiTableColumnFlags>(lhs)
                                    | static_cast<ImGuiTableColumnFlags>(rhs));
}

struct TableColumnConfig {
  const char* Name;
  TableColumnFlag Flags;
};

struct Table : Component<"Table"> {
 public:
  Table(RenderCtx& renderCtx,
        const char* tableName,
        TableFlag flags = TableFlag::kNone,
        std::initializer_list<TableColumnConfig> columns = {});
  Table(RenderCtx& renderCtx,
        const char* tableName,
        TableFlag flags = TableFlag::kNone,
        std::span<const TableColumnConfig> columns = {});
  Table(RenderCtx& renderCtx,
        const std::string& tableName,
        TableFlag flags = TableFlag::kNone,
        std::initializer_list<TableColumnConfig> columns = {});
  Table(RenderCtx& renderCtx,
        const std::string& tableName,
        TableFlag flags = TableFlag::kNone,
        std::span<const TableColumnConfig> columns = {});
  ~Table();

  Table(const Table&) = delete;
  auto operator=(const Table&) -> Table& = delete;
  Table(Table&&) = delete;
  auto operator=(Table&&) -> Table& = delete;

  void AddRow(std::span<char const * const> data) const;
  void AddRow(std::span<const std::string> data) const;

 private:
  bool isRendering_ = false;
};

} // namespace immm

#endif // IMMM_TABLE_HPP
