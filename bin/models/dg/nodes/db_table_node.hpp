#ifndef IMSQL_MODELS_DG_NODES_DB_TABLE_NODE_HPP
#define IMSQL_MODELS_DG_NODES_DB_TABLE_NODE_HPP

#include "models/db_model.hpp"
#include "models/dg/nodes/base_node.hpp"
#include <memory>

namespace imsql::dg {

class DbTableNode;

class DbColumnVertex final : public BaseVertex {
public:
  explicit DbColumnVertex(
    DesignGraphModel* model,
    const DbColumnModel* column
  ) noexcept
      : BaseVertex(model), column_(column) {}

  [[nodiscard]] constexpr auto Name() const noexcept -> std::string_view override {
    return column_->Name();
  }

  [[nodiscard]] constexpr auto GetValue(std::size_t rowId) const -> std::optional<Value> override {
    return column_->GetValue(rowId);
  }

  [[nodiscard]] constexpr auto GetAllValues() const -> std::vector<Value> override {
    // @todo(marko): Using std::string here is definitely a hack.
    return column_->GetRows();
  }

private:
  const DbColumnModel* column_;
};

class DbTableNode final : public BaseNode {
public:
  explicit DbTableNode(DesignGraphModel* model, DbTableModel* table) noexcept
      : BaseNode(model), table_(table) {}

  [[nodiscard]] auto Name() const noexcept -> std::string_view override {
    return table_->Name();
  }

  constexpr void AddVertex(DbColumnVertex* vertex) {
    vertices_.push_back(vertex);
  }

private:
  DbTableModel* table_;
  std::vector<DbColumnVertex*> vertices_;

  friend class DbColumnModel;
};


} // namespace imsql::dg

#endif // IMSQL_MODELS_DG_NODES_DB_TABLE_NODE_HPP
