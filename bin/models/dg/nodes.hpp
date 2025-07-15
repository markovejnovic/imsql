#ifndef IMSQL_MODELS_DG_NODES_HPP
#define IMSQL_MODELS_DG_NODES_HPP

#include "pch.hpp"
#include "models/db_model.hpp"

namespace imsql::dg {

class DbColumnVertex final {
public:
  constexpr explicit DbColumnVertex(const DbColumnModel* column) noexcept : column_(column) {}

  [[nodiscard]] constexpr auto Name() const noexcept -> std::string_view {
    return column_->Name();
  }

  [[nodiscard]] constexpr auto Type() const noexcept -> std::string_view {
    return column_->Type();
  }

  [[nodiscard]] constexpr auto Value(std::size_t rowId) const -> std::string {
    return column_->GetValue<std::string>(rowId);
  }

private:
  const DbColumnModel* column_;
};

class DbNode {};

class DbTableNode final : public DbNode {
public:
  constexpr explicit DbTableNode(DbTableModel* table) noexcept : table_(table) {};

  [[nodiscard]] auto Name() const noexcept -> std::string_view {
    return table_->Name();
  }

  /// @brief Retrieve all the values for the given column in the table.
  template <class ValueType>
  [[nodiscard]] auto ValuesForColumn(std::string_view column_name) const -> std::vector<ValueType>;

  [[nodiscard]] auto Value(std::string_view column, std::size_t rowId) const -> std::string;

  /// @todo This will get removed with a cleaner interface.
  [[nodiscard]] constexpr auto Model() -> DbTableModel* {
    return table_;
  }

  [[nodiscard]] constexpr auto Model() const -> const DbTableModel* {
    return table_;
  }

private:
  DbTableModel* table_;
};

class SpreadsheetNode final {
public:
  [[nodiscard]] auto Name() const noexcept -> std::string_view {
    return "Main Spreadsheet";
  }
};

class OperatorNode { };

class TransformNode final : public OperatorNode {
public:
  class InKeyVtx {
  public:
    InKeyVtx(unsigned long id, const TransformNode* node) noexcept
      : id_(id), node_(node) {}

    [[nodiscard]] constexpr auto Value(std::size_t rowId) const -> std::size_t {
    }

  private:
    const TransformNode* node_;
    unsigned long id_;
  };

  class InValueVtx {
  public:
    InValueVtx(unsigned long id, const TransformNode* node) noexcept
      : id_(id), node_(node) {}

    [[nodiscard]] constexpr auto Value(std::size_t rowId) const -> std::string {
    }

  private:
    const TransformNode* node_;
    unsigned long id_;
  };

  class OutValueVtx {
  public:
    OutValueVtx(unsigned long id, const TransformNode* node) noexcept
      : id_(id), node_(node) {}

    [[nodiscard]] constexpr auto Value(std::size_t rowId) const -> std::string {
      // The output vertex returns the value out of the InValueVtx, keyed via the InKeyVtx.
      return node_->InputValueVertex().Value(node_->InputKeyVertex().Value(rowId));
    }

  private:
    const TransformNode* node_;
    unsigned long id_;
  };

  void SetVertices(
    unsigned long inKeyVtx,
    unsigned long inValueVtx,
    unsigned long outValueVtx) noexcept {
    inKeyVtx_ = inKeyVtx;
    inValueVtx_ = inValueVtx;
    outValueVtx_ = outValueVtx;
  }

  [[nodiscard]] auto Name() const noexcept -> std::string_view {
    return "Transform";
  }

  [[nodiscard]] constexpr auto InputKeyVertex() const noexcept -> InKeyVtx {
    return {inKeyVtx_, this};
  }

  [[nodiscard]] constexpr auto InputValueVertex() const noexcept -> InValueVtx {
    return {inValueVtx_, this};
  }

  [[nodiscard]] constexpr auto OutputValueVertex() const noexcept -> OutValueVtx {
    return {outValueVtx_, this};
  }

private:
  // @todo(marko): Two hacks: using unsigned long instead of VertexType and using max value to
  // indicate that the ID is not set.
  unsigned long inKeyVtx_ = std::numeric_limits<unsigned long>::max();
  unsigned long inValueVtx_ = std::numeric_limits<unsigned long>::max();
  unsigned long outValueVtx_ = std::numeric_limits<unsigned long>::max();
};

class Node {
public:
  using VariantType = std::variant<DbTableNode, TransformNode, SpreadsheetNode>;

  explicit Node(const VariantType& variant) : variant_(variant) {}

  [[nodiscard]] constexpr auto Name() const noexcept -> std::string_view {
    return std::visit([](const auto& node) { return node.Name(); }, variant_);
  }

  [[nodiscard]] constexpr auto Variant() const noexcept -> const VariantType& {
    return variant_;
  }

  template <class T>
  [[nodiscard]] constexpr auto As() const noexcept -> const T& {
    return std::get<T>(variant_);
  }

  template <class T>
  [[nodiscard]] constexpr auto As() noexcept -> T& {
    return std::get<T>(variant_);
  }

  [[nodiscard]] constexpr auto Variant() noexcept -> VariantType& {
    return variant_;
  }

private:
  VariantType variant_;
};

} // namespace imsql::dg

#endif // IMSQL_MODELS_DG_NODES_HPP
