#include "immm/nodes/output_attribute.hpp"
#include "immm/detail/pch.hpp"
#include "immm/render_ctx.hpp"

namespace immm::nodes {

OutputAttribute::OutputAttribute(RenderCtx& renderCtx, int attributeId) : Component(renderCtx) {
  ImNodes::BeginOutputAttribute(attributeId);
}

OutputAttribute::~OutputAttribute() {
  ImNodes::EndOutputAttribute();
}

}  // namespace immm::nodes
