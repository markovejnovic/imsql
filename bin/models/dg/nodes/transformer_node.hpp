#ifndef IMSQL_MODELS_DG_NODES_TRANSFORMER_NODE_HPP
#define IMSQL_MODELS_DG_NODES_TRANSFORMER_NODE_HPP

#include "models/dg/graph_types.hpp"
#include "models/dg/nodes/base_node.hpp"
namespace imsql::dg {

class TransformNode;

class TransformInputValueVertex final : public BaseVertex {
public:
  TransformInputValueVertex(
    DesignGraphModel* model,
    TransformNode* node) : BaseVertex(model), node_(node) {}

  ~TransformInputValueVertex() override;

  [[nodiscard]] auto Name() const noexcept -> std::string_view override {
    return "value";
  }

  [[nodiscard]] auto GetValue(std::size_t rowId) const -> std::optional<Value> override {
    return GetInputVertex()->GetValue(rowId);
  }

  [[nodiscard]] auto GetAllValues() const -> std::vector<Value> override {
    // @todo(marko): This is debt.
    throw std::runtime_error("GetAllValues is not supported for TransformInputValueVertex.");
  }

private:
  TransformNode* node_;
};

class TransformInputKeyVertex final : public BaseVertex {
public:
  TransformInputKeyVertex(
    DesignGraphModel* model,
    TransformNode* node) : BaseVertex(model), node_(node) {}

  ~TransformInputKeyVertex() override;

  [[nodiscard]] auto Name() const noexcept -> std::string_view override {
    return "key";
  }

  [[nodiscard]] auto GetValue(std::size_t rowId) const -> std::optional<Value> override {
    return GetInputVertex()->GetValue(rowId);
  }

  [[nodiscard]] auto GetAllValues() const -> std::vector<Value> override {
    // @todo(marko): This is debt.
    throw std::runtime_error("GetAllValues is not supported for TransformInputKeyVertex.");
  }

private:
  TransformNode* node_;
};

class TransformOutputVertex final : public BaseVertex {
public:
  TransformOutputVertex(
    DesignGraphModel* model,
    TransformNode* node) : BaseVertex(model), node_(node) {}

  ~TransformOutputVertex() override;

  [[nodiscard]] auto Name() const noexcept -> std::string_view override {
    return "output";
  }

  [[nodiscard]] auto GetValue(std::size_t rowId) const -> std::optional<Value> override;

  [[nodiscard]] auto GetAllValues() const -> std::vector<Value> override {
    // @todo(marko): This is debt.
    throw std::runtime_error("GetAllValues is not supported for TransformOutputVertex.");
  }

private:
  [[nodiscard]] auto inputKeyVertex() const -> BaseVertex*;
  [[nodiscard]] auto inputValueVertex() const -> BaseVertex*;

  TransformNode* node_;
};

class TransformNode final : public BaseNode {
public:
  explicit TransformNode(DesignGraphModel* model) : BaseNode(model) {}

  [[nodiscard]] auto Name() const noexcept -> std::string_view override {
    return "Transform";
  }

  void SetInputKeyVertex(TransformInputKeyVertex* vertex) {
    inputKeyVertex_ = vertex;
  }

  void SetInputValueVertex(TransformInputValueVertex* vertex) {
    inputValueVertex_ = vertex;
  }

  void SetOutputVertex(TransformOutputVertex* vertex) {
    outputVertex_ = vertex;
  }

private:
  TransformInputKeyVertex* inputKeyVertex_ = nullptr;
  TransformInputValueVertex* inputValueVertex_ = nullptr;
  TransformOutputVertex* outputVertex_ = nullptr;

  friend class TransformOutputVertex;
};

} // namespace imsql::dg

#endif // IMSQL_MODELS_DG_NODES_TRANSFORMER_NODE_HPP
