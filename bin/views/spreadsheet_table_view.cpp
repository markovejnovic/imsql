#include "spreadsheet_table_view.hpp"
#include "immm/table.hpp"
#include "models/dg/nodes/base_node.hpp"

namespace imsql {

void SpreadsheetTableView::operator()(immm::RenderCtx& ctx) {
  //// We need to collect all the columns before we can render the table.
  const auto column_configs =
    std::ranges::to<std::vector<immm::TableColumnConfig>>(
      model_->Vertices(model_->GetSpreadsheetNode())
      | std::views::transform([&](const dg::VertexType& vtx) {
          return model_->VertexName(vtx);
        })
      | std::views::transform([](std::string_view name) {
          return immm::TableColumnConfig {
            .Name = std::string(name).c_str(),
            .Flags = immm::TableColumnFlag::kWidthStretch,
          };
        }
      )
    );

  immm::Table table{ctx, "Spreadsheet Table",
                    immm::TableFlag::kResizable,
                    std::span<const immm::TableColumnConfig>(column_configs)};

  for (const auto& row_id : model_->GetRowIds()) {
    //std::vector columnar_data = model_->GetSpreadsheetValues(row_id);
    auto row_values = std::ranges::to<std::vector<std::string>>(
      model_->GetSpreadsheetNode()->GetRowValues(row_id)
        | std::views::transform([](const std::optional<dg::Value>& val) {
            if (!val.has_value()) {
              return std::string("");
            }

            return std::format("{}", *val);
          }));
    table.AddRow(std::span<const std::string>(row_values));
  }
}

} // namespace imsql
