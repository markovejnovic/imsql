#include "imsqlite/ui/node_editor/node.hpp"
#include "imgui.h"
#include "imnodes.h"

namespace imsql::ui {

Node::Node(int id, const char* title) {
  ImNodes::BeginNode(id);

  ImNodes::BeginNodeTitleBar();
  ImGui::TextUnformatted(title);
  ImNodes::EndNodeTitleBar();
}

Node::~Node() {
  ImNodes::EndNode();
}

void Node::AddInput(int inputId, const char* label) {
  ImNodes::BeginInputAttribute(inputId);
  ImGui::TextUnformatted(label);
  ImNodes::EndInputAttribute();
}

void Node::AddOutput(int outputId, const char* label) {
  ImNodes::BeginOutputAttribute(outputId);
  ImGui::TextUnformatted(label);
  ImNodes::EndOutputAttribute();
}

} // namespace imsql::ui
