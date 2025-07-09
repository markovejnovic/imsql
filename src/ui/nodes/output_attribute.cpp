#include "imsqlite/ui/nodes/output_attribute.hpp"
#include "imnodes.h"
#include "imsqlite/ui/render_ctx.hpp"

namespace imsql::ui::nodes {

OutputAttribute::OutputAttribute(RenderCtx& renderCtx, int attributeId)
    : Component(renderCtx) {
  ImNodes::BeginOutputAttribute(attributeId);
}

OutputAttribute::~OutputAttribute() {
  ImNodes::EndOutputAttribute();
}

} // namespace imsql::ui::nodes
