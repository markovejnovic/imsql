#include "immm/nodes/input_attribute.hpp"
#include "immm/detail/pch.hpp"
#include "immm/render_ctx.hpp"

namespace immm::nodes {

InputAttribute::InputAttribute(RenderCtx& renderCtx, int attributeId) : Component(renderCtx) {
  ImNodes::BeginInputAttribute(attributeId);
}

InputAttribute::~InputAttribute() {
  ImNodes::EndInputAttribute();
}

}  // namespace immm::nodes
