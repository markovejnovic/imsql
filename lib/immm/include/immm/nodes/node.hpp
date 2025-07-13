#ifndef IMMM_NODES_NODE_HPP
#define IMMM_NODES_NODE_HPP

#include "immm/base_types.hpp"
#include "immm/component.hpp"
#include "immm/render_ctx.hpp"

namespace immm::nodes {

class Node final : Component<"Node"> {
 public:
  explicit Node(RenderCtx& renderCtx, int nodeId);
  Node(RenderCtx& renderCtx, int nodeId, const Vec2& position);
  ~Node();

  void SetPosition(const Vec2& position) const noexcept;
  [[nodiscard]] auto Position() const -> Vec2;

  [[nodiscard]] constexpr auto Id() const noexcept -> int { return nodeId_; }

  Node(const Node&) = delete;
  auto operator=(const Node&) -> Node& = delete;
  Node(Node&&) = delete;
  auto operator=(Node&&) -> Node& = delete;

 private:
  int nodeId_;
};

}  // namespace immm::nodes

#endif  // IMMM_NODES_NODE_HPP
