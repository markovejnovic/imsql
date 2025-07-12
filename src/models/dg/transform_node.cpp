#include "imsqlite/models/dg/transform_node.hpp"
#include <array>
#include <string_view>

namespace imsql::models::dg {

static constexpr std::array<std::string_view, 2> kInputFields = {"key", "value"};
static constexpr std::array<std::string_view, 1> kOutputFields = {"output"};

auto TransformNode::Name() const noexcept -> std::string_view {
  return "Transform";
}

auto TransformNode::InputFields() const noexcept -> std::span<const std::string_view> {
  return kInputFields;
}

auto TransformNode::OutputFields() const noexcept -> std::span<const std::string_view> {
  return kOutputFields;
}

} // namespace imsql::models::dg
