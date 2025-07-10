#include "imsqlite/ui/nodes/static_attribute.hpp"
#include "imsqlite/pch/imnodes.hpp"
#include "imsqlite/ui/render_ctx.hpp"

namespace imsql::ui::nodes {

StaticAttribute::StaticAttribute(RenderCtx& renderCtx, int attributeId)
    : Component(renderCtx) {
  ImNodes::BeginStaticAttribute(attributeId);
}

StaticAttribute::~StaticAttribute() {
  ImNodes::EndStaticAttribute();
}

} // namespace imsql::ui::nodes
