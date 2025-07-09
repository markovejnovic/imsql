#include "imsqlite/ui/nodes/link.hpp"
#include "imnodes.h"
#include "imsqlite/ui/render_ctx.hpp"

namespace imsql::ui::nodes {

Link::Link(RenderCtx& ctx, int linkId, int startAttributeId, int endAttributeId)
    : Component(ctx) {
  ImNodes::Link(linkId, startAttributeId, endAttributeId);
}

} // namespace imsql::ui::nodes
