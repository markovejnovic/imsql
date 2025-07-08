#include "imsqlite/ui/nodes/output_attribute.hpp"
#include "imnodes.h"
#include "imsqlite/ui/render_ctx.hpp"

namespace imsql::ui::nodes {

constexpr OutputAttribute::OutputAttribute(RenderCtx& renderCtx, int attributeId)
    : Component(renderCtx) {
  ImNodes::BeginOutputAttribute(attributeId);
}

constexpr OutputAttribute::~OutputAttribute() {
  ImNodes::EndOutputAttribute();
}

} // namespace imsql::ui::nodes
