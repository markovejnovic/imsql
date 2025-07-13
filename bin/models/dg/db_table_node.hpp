#ifndef IMSQLITE_MODELS_DG_DB_TABLE_NODE_HPP
#define IMSQLITE_MODELS_DG_DB_TABLE_NODE_HPP

#include "db_node.hpp"
#include "models/db_model.hpp"

namespace imsql::dg {

class DbTableNode final : public DbNode {
public:
  constexpr explicit DbTableNode(DbTableModel* table) noexcept : table_(table) {};

  [[nodiscard]] auto Name() const noexcept -> std::string_view override {
    return table_->Name();
  }

private:
  DbTableModel* table_;
};

} // namespace imsql::dg

#endif // IMSQLITE_MODELS_DG_DB_TABLE_NODE_HPP
