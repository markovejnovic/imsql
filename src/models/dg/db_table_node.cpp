#include "imsqlite/models/dg/db_table_node.hpp"

namespace imsql::models::dg {

DbTableNode::DbTableNode(const models::Db& dbModel, TableId tableId)
      : DbNode(dbModel), tableId_(tableId) {};

auto DbTableNode::Name() const noexcept -> std::string_view {
  return dbModel_->TableNameSV(tableId_);
}

} // namespace imsql::models::dg
