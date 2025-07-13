#ifndef IMSQLITE_MODELS_DG_TRANSFORM_NODE_HPP
#define IMSQLITE_MODELS_DG_TRANSFORM_NODE_HPP

#include "operator_node.hpp"

namespace imsql::dg {

class TransformNode final : public OperatorNode {
public:
  [[nodiscard]] auto Name() const noexcept -> std::string_view override;

  [[nodiscard]] auto InputFields() const noexcept -> std::span<const std::string_view> override;
  [[nodiscard]] auto OutputFields() const noexcept -> std::span<const std::string_view> override;
};

} // namespace imsql::dg

#endif // IMSQLITE_MODELS_DG_TRANSFORM_NODE_HPP
