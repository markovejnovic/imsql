#include "imsqlite/ui/node_editor/node_editor.hpp"

#include "imnodes.h"

namespace imsql::ui {

NodeEditor::NodeEditor() {
  ImNodes::BeginNodeEditor();
}

NodeEditor::~NodeEditor() {
  ImNodes::EndNodeEditor();
}

} // namespace imsql::ui
