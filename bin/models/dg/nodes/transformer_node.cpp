#include "transformer_node.hpp"
#include "util/fp.hpp"

namespace imsql::dg {

TransformInputKeyVertex::~TransformInputKeyVertex() = default;
TransformInputValueVertex::~TransformInputValueVertex() = default;

TransformOutputVertex::~TransformOutputVertex() = default;

auto TransformOutputVertex::inputKeyVertex() const -> BaseVertex* {
  return node_->inputKeyVertex_;
}

auto TransformOutputVertex::inputValueVertex() const -> BaseVertex* {
  return node_->inputValueVertex_;
}

auto TransformOutputVertex::GetValue(std::size_t rowId) const -> std::optional<Value> {
  const auto& keyValue = inputKeyVertex()->GetValue(rowId);
  if (!keyValue.has_value()) {
    return std::nullopt;
  }

  return std::visit(util::Overloads {
    [](const NullValue&) -> std::optional<Value> {
      // If the key is null, the best we can do is propagate the value.
      return NullValue{};
    },
    [](const StringValue&) -> std::optional<Value> {
      // If the key is a string, we do not know how to handle that right now, so we simply exit.
      // @todo(marko): Add support for string keys
      throw std::runtime_error("String keys are not supported in TransformOutputVertex.");
    },
    [&](const Int64Value& int64Value) -> std::optional<Value> {
      // If the key is an integer, we can use it to retrieve the value.
      return inputValueVertex()->GetValue(int64Value.Value());
    }
  }, *keyValue);
}

} // namespace imsql::dg
