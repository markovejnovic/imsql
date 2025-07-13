#ifndef IMSQLITE_MODELS_DG_OPERATOR_NODE_HPP
#define IMSQLITE_MODELS_DG_OPERATOR_NODE_HPP

#include "node.hpp"

namespace imsql::dg {

class OperatorNode : public Node {
public:
  [[nodiscard]] virtual auto InputFields() const noexcept -> std::span<const std::string_view> = 0;
  [[nodiscard]] virtual auto OutputFields() const noexcept -> std::span<const std::string_view> = 0;
};

} // namespace imsql::dg

#endif // IMSQLITE_MODELS_DG_OPERATOR_NODE_HPP
