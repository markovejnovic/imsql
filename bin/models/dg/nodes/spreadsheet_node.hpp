#ifndef IMSQL_DG_NODES_SPREADSHEET_NODE_HPP
#define IMSQL_DG_NODES_SPREADSHEET_NODE_HPP

#include "models/dg/nodes/base_node.hpp"
#include <cstdlib>

namespace imsql::dg {

class SpreadsheetVertex final : public BaseVertex {
public:
  explicit SpreadsheetVertex(
    DesignGraphModel* model,
    std::string name
  ) : BaseVertex(model), name_(std::move(name)) {}

  [[nodiscard]] auto Name() const noexcept -> std::string_view override {
    return name_;
  }

  [[nodiscard]] auto GetValue(std::size_t rowId) const -> std::optional<Value> override {
    const BaseVertex* ip_vtx = GetInputVertex();
    if (ip_vtx == nullptr) {
      // If the input vertex does not exist, that means that the user didn't connect an edge to the
      // current vertex. In this case, we return an empty optional.
      return std::nullopt;
    }

    return ip_vtx->GetValue(rowId);
  }

  [[nodiscard]] auto GetAllValues() const -> std::vector<Value> override {
    // To fetch all the values of this column, we need to get its input vertex. If the input vertex
    // does not exist, that means that the user didn't connect an edge to the current vertex. In
    // this case, there are no values.
    const BaseVertex* inp_vertex = GetInputVertex();
    if (inp_vertex == nullptr) {
      return {};
    }

    // Otherwise, simply fetch all the values from the input vertex.
    return inp_vertex->GetAllValues();
  }

private:
  std::string name_;
};

class SpreadsheetNode final : public BaseNode {
public:
  explicit SpreadsheetNode(DesignGraphModel* model) noexcept
      : BaseNode(model) {}

  [[nodiscard]] auto Name() const noexcept -> std::string_view override {
    return "Spreadsheet";
  }

  [[nodiscard]] auto GetRowValues(std::size_t rowId) const -> std::vector<std::optional<Value>> {
    return Vertices()
      | std::views::transform([&](const BaseVertex* vtx) {
          return vtx->GetValue(rowId);
        })
      | std::ranges::to<std::vector<std::optional<Value>>>();
  }
};

} // namespace imsql::dg

#endif // IMSQL_DG_NODES_SPREADSHEET_NODE_HPP
