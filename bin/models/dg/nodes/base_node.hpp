#ifndef IMSQL_MODELS_DG_NODES_BASE_NODE_HPP
#define IMSQL_MODELS_DG_NODES_BASE_NODE_HPP

#include "models/dg/nodes/values.hpp"
#include "models/dg/graph_types.hpp"
#include <span>

namespace imsql::dg {

class DesignGraphModel;

class BaseVertex {
public:
  explicit BaseVertex(DesignGraphModel* graph, VertexType vertexId = 0);
  virtual ~BaseVertex() = default;

  BaseVertex(const BaseVertex&) = default;
  auto operator=(const BaseVertex&) -> BaseVertex& = default;
  BaseVertex(BaseVertex&&) = default;
  auto operator=(BaseVertex&&) -> BaseVertex& = default;

  /// @brief Get the name of the vertex.
  [[nodiscard]] virtual auto Name() const noexcept -> std::string_view = 0;

  [[nodiscard]] virtual auto GetValue(std::size_t rowId) const -> std::optional<Value> = 0;

  /// @todo(marko): Returning a vector of "Value" is kind of dumb for columnar data considering all
  /// columns share the same type, but this is a temporary solution.
  [[nodiscard]] virtual auto GetAllValues() const -> std::vector<Value> = 0;

  /// @brief Retrieve the vertex looking into this vertex.
  /// @note This function will assert that there is only one vertex.
  [[nodiscard]] auto GetInputVertex() const noexcept -> const BaseVertex*;

  [[nodiscard]] constexpr auto Graph() noexcept {
    return graph_;
  }

  [[maybe_unused]] constexpr auto SetVertexId(VertexType vertexId) noexcept {
    vertexId_ = vertexId;
    return vertexId_;
  }

  [[nodiscard]] constexpr auto VertexId() const noexcept -> VertexType {
    return vertexId_;
  }

private:
  DesignGraphModel* graph_;
  VertexType vertexId_;
};

class BaseNode {
 public:
  explicit BaseNode(DesignGraphModel* graph) : graph_(graph) {};
  virtual ~BaseNode() = default;

  BaseNode(const BaseNode&) = default;
  auto operator=(const BaseNode&) -> BaseNode& = default;
  BaseNode(BaseNode&&) = default;
  auto operator=(BaseNode&&) -> BaseNode& = default;

  /// @brief Get the name of the node.
  [[nodiscard]] virtual auto Name() const noexcept -> std::string_view = 0;

  [[nodiscard]] auto Vertices() const noexcept -> std::vector<const BaseVertex*>;

protected:
  [[nodiscard]] auto graph() const noexcept -> const DesignGraphModel* {
    return graph_;
  }

  [[nodiscard]] auto graph() noexcept -> DesignGraphModel* {
    return graph_;
  }

private:
  DesignGraphModel* graph_;
};

} // namespace imsql::dg

#endif // IMSQL_MODELS_DG_NODES_BASE_NODE_HPP
