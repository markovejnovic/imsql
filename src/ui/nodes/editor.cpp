#include "imsqlite/ui/nodes/editor.hpp"
#include "imnodes.h"
#include "imsqlite/ui/render_ctx.hpp"

namespace imsql::ui::nodes {

Editor::Editor(RenderCtx& renderCtx) : Component(renderCtx) {
  ImNodes::BeginNodeEditor();
}

Editor::~Editor() {
  ImNodes::EndNodeEditor();
}

} // namespace imsql::ui::nodes
