#include "models/dg/nodes.hpp"

namespace imsql::dg {

template <>
auto DbTableNode::ValuesForColumn<std::size_t>(
  std::string_view column_name
) const -> std::vector<std::size_t> {
  return table_->ColumnByName(column_name).GetRows<std::size_t>();
}

auto DbTableNode::Value(
  std::string_view column,
  std::size_t rowId
) const -> std::string {
  // @todo(marko): This has horrible performance obviously, but it is a quick implementation.
  return table_->GetValue<std::string>(column, rowId);
}

} // namespace imsql::dg 
