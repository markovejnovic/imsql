#ifndef IMSQLITE_MODELS_DG_DB_TABLE_NODE_HPP
#define IMSQLITE_MODELS_DG_DB_TABLE_NODE_HPP

#include "db_node.hpp"

namespace imsql::models::dg {

class DbTableNode final : public DbNode {
public:
  explicit DbTableNode(const models::Db& dbModel, TableId tableId);

  [[nodiscard]] auto Name() const noexcept -> std::string_view override;

private:
  models::TableId tableId_;
};

} // namespace imsql::models::dg

#endif // IMSQLITE_MODELS_DG_DB_TABLE_NODE_HPP
