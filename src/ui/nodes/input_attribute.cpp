#include "imsqlite/ui/nodes/input_attribute.hpp"
#include "imsqlite/pch/imnodes.hpp"
#include "imsqlite/ui/render_ctx.hpp"

namespace imsql::ui::nodes {

InputAttribute::InputAttribute(RenderCtx& renderCtx, int attributeId)
    : Component(renderCtx) {
  ImNodes::BeginInputAttribute(attributeId);
}

InputAttribute::~InputAttribute() {
  ImNodes::EndInputAttribute();
}

} // namespace imsql::ui::nodes
