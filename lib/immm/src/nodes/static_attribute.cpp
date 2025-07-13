#include "immm/nodes/static_attribute.hpp"
#include "immm/detail/pch.hpp"
#include "immm/render_ctx.hpp"

namespace immm::nodes {

StaticAttribute::StaticAttribute(RenderCtx& renderCtx, int attributeId) : Component(renderCtx) {
  ImNodes::BeginStaticAttribute(attributeId);
}

StaticAttribute::~StaticAttribute() {
  ImNodes::EndStaticAttribute();
}

}  // namespace immm::nodes
