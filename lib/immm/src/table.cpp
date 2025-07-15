#include "imgui.h"
#include "immm/detail/pch.hpp"
#include "immm/table.hpp"
#include <iostream>

namespace immm {

Table::Table(
  RenderCtx& renderCtx,
  const char* tableName,
  TableFlag flags,
  std::span<const TableColumnConfig> columns)
    : Component(renderCtx),
      isRendering_(
        ImGui::BeginTable(tableName, gsl::narrow<int>(columns.size()), static_cast<int>(flags))
      ) {
  if (!isRendering_) {
    return;
  }

  for (const auto& column : columns) {
    ImGui::TableSetupColumn(column.Name, static_cast<ImGuiTableColumnFlags>(column.Flags));
  }

  ImGui::TableHeadersRow();
}

Table::Table(
  RenderCtx& renderCtx,
  const char* tableName,
  TableFlag flags,
  std::initializer_list<TableColumnConfig> columns)
    : Table(renderCtx, tableName, flags, std::span<const TableColumnConfig>(columns)) {}

Table::Table(
  RenderCtx& renderCtx,
  const std::string& tableName,
  TableFlag flags,
  std::span<const TableColumnConfig> columns)
    : Table(renderCtx, tableName.c_str(), flags, columns) {}

Table::Table(
  RenderCtx& renderCtx,
  const std::string& tableName,
  TableFlag flags,
  std::initializer_list<TableColumnConfig> columns)
    : Table(renderCtx, tableName.c_str(), flags, std::span<const TableColumnConfig>(columns)) {}

Table::~Table() {
  if (isRendering_) {
    ImGui::EndTable();
  }
}

void Table::AddRow(std::span<char const * const> data) const {
  if (!isRendering_) {
    return;
  }

  ImGui::TableNextRow();
  for (const auto& item : data) {
    ImGui::TableNextColumn();
    ImGui::TextUnformatted(item);
  }
}

void Table::AddRow(std::span<const std::string> data) const {
  if (!isRendering_) {
    return;
  }

  ImGui::TableNextRow();
  for (const auto& item : data) {
    ImGui::TableNextColumn();
    ImGui::TextUnformatted(item.c_str());
  }
}

} // namespace immm
