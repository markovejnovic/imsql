#include "immm/nodes/node.hpp"
#include "immm/detail/imgui_interop.hpp"
#include "immm/detail/pch.hpp"

namespace immm::nodes {

Node::Node(RenderCtx& renderCtx, int nodeId) : Component(renderCtx), nodeId_(nodeId) {
  ImNodes::BeginNode(nodeId_);
}

Node::Node(RenderCtx& renderCtx, int nodeId, const Vec2& position) : Node(renderCtx, nodeId) {
  SetPosition(position);
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

}  // namespace immm::nodes
