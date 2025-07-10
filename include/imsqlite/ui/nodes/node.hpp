#ifndef IMSQLITE_UI_NODES_NODE_HPP
#define IMSQLITE_UI_NODES_NODE_HPP

#include "imsqlite/ui/base_types.hpp"
#include "imsqlite/ui/component.hpp"
#include "imsqlite/ui/render_ctx.hpp"

namespace imsql::ui::nodes {

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

} // namespace imsql::ui::nodes

#endif // IMSQLITE_UI_NODES_NODE_HPP
