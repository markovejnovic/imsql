#include "immm/nodes/editor.hpp"
#include "immm/detail/pch.hpp"
#include "immm/render_ctx.hpp"

namespace immm::nodes {

Editor::Editor(RenderCtx& renderCtx) : Component(renderCtx) {
  ImNodes::BeginNodeEditor();
}

Editor::~Editor() {
  ImNodes::EndNodeEditor();
}

}  // namespace immm::nodes
