#include "imsqlite/ui/nodes/node.hpp"
#include "imnodes.h"
#include "imsqlite/ui/detail/imgui_interop.hpp"

namespace imsql::ui::nodes {

Node::Node(RenderCtx& renderCtx, int nodeId)
    : Component(renderCtx), nodeId_(nodeId) {
  ImNodes::BeginNode(nodeId_);
}

Node::~Node() {
  ImNodes::EndNode();
}

void Node::SetPosition(const Vec2& position) const noexcept {
  ImNodes::SetNodeGridSpacePos(nodeId_, detail::Vec2ToImVec2(position));
}

auto Node::Position() const -> Vec2 {
  return detail::ImVec2ToVec2(ImNodes::GetNodeGridSpacePos(nodeId_));
}

} // namespace imsql::ui::nodes
