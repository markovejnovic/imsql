#ifndef IMSQLITE_DSALG_SPARSE_TREE_HPP
#define IMSQLITE_DSALG_SPARSE_TREE_HPP

#include <vector>

namespace imsql::dsalg {

/// @brief Horrible implementation of a tree. Memory is laid out sparsely.
template <class ValueT>
class SparseTreeNode {
public:
  using ValueType = ValueT;

  explicit constexpr SparseTreeNode(ValueType value)
    : value_(std::move(value)) {}

  [[nodiscard]] constexpr auto Value() const noexcept -> const ValueType& {
    return value_;
  }

  [[nodiscard]] constexpr auto Children() const noexcept
      -> const std::vector<SparseTreeNode>& {
    return children_;
  }

  [[nodiscard]] constexpr auto Children() noexcept
      -> std::vector<SparseTreeNode>& {
    return children_;
  }

  void AddChild(SparseTreeNode child) {
    children_.emplace_back(std::move(child));
  }

  [[nodiscard]] constexpr auto IsLeaf() const noexcept -> bool {
    return children_.empty();
  }

private:
  ValueType value_;
  std::vector<SparseTreeNode> children_;
};

} // namespace imsql::dsalg

#endif // IMSQLITE_DSALG_SPARSE_TREE_HPP
