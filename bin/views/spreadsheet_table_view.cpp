#include "spreadsheet_table_view.hpp"
#include "immm/table.hpp"
#include "models/dg/nodes.hpp"

namespace imsql {

void SpreadsheetTableView::operator()(immm::RenderCtx& ctx) {
  // We need to collect all the columns before we can render the table.
  const auto column_configs =
    std::ranges::to<std::vector<immm::TableColumnConfig>>(
      model_->Vertices(model_->GetSpreadsheetNode())
      | std::views::transform([&](const dg::DesignGraphModel::VertexType& vtx) {
          return model_->VertexName(vtx).c_str();
        })
      | std::views::transform([](const char* name) {
          return immm::TableColumnConfig {
            .Name = name,
            .Flags = immm::TableColumnFlag::kWidthStretch,
          };
        }
      )
    );

  immm::Table table{ctx, "Spreadsheet Table",
                    immm::TableFlag::kResizable,
                    std::span<const immm::TableColumnConfig>(column_configs)};

  for (const auto& row_id : model_->GetRowIds()) {
    std::vector columnar_data = model_->GetSpreadsheetValues(row_id);
    table.AddRow(std::span<const std::string>(columnar_data));
  }
}

} // namespace imsql
