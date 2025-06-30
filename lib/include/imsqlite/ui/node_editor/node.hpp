#ifndef IMSQLITE_UI_NODE_EDITOR_NODE_HPP
#define IMSQLITE_UI_NODE_EDITOR_NODE_HPP
#pragma once

namespace imsql::ui {

class Node {
public:
  Node(int id, const char* title);
  ~Node();

  Node(const Node&) = delete;
  auto operator=(const Node&) -> Node& = delete;
  Node(Node&&) = delete;
  auto operator=(Node&&) -> Node& = delete;

  void AddInput(int inputId, const char* label);
  void AddOutput(int outputId, const char* label);
};

} // namespace imsql::ui

#endif // IMSQLITE_UI_NODE_EDITOR_NODE_HPP
