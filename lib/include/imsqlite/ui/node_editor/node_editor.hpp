#ifndef IMSQLITE_UI_NODE_EDITOR_NODE_EDITOR_HPP
#define IMSQLITE_UI_NODE_EDITOR_NODE_EDITOR_HPP
#pragma once

#include "node.hpp"

namespace imsql::ui {

class NodeEditor {
public:
  NodeEditor();
  ~NodeEditor();

  NodeEditor(const NodeEditor&) = delete;
  auto operator=(const NodeEditor&) -> NodeEditor& = delete;
  NodeEditor(NodeEditor&&) = delete;
  auto operator=(NodeEditor&&) -> NodeEditor& = delete;

  auto CreateNode(int nodeId, const char* title) -> Node {
    return {nodeId, title};
  }
};

} // namespace imsql::ui

#endif // IMSQLITE_UI_NODE_EDITOR_NODE_EDITOR_HPP
