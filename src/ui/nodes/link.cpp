#include "imsqlite/ui/nodes/link.hpp"
#include "imsqlite/pch/imnodes.hpp"
#include "imsqlite/ui/nodes/color_style.hpp"
#include "imsqlite/ui/render_ctx.hpp"
#include <optional>

namespace imsql::ui::nodes {

Link::Link(RenderCtx& ctx, int linkId, int startAttributeId, int endAttributeId)
    : Component(ctx) {
  ImNodes::Link(linkId, startAttributeId, endAttributeId);
}

Link::Link(RenderCtx& ctx, int linkId, int startAttributeId, int endAttributeId,
           LinkColorStyle colorStyle) : Component(ctx) {
  ColorStyle color_style {
    ImNodesCol_Link, static_cast<ImU32>(colorStyle.Normal),
    ImNodesCol_LinkSelected, static_cast<ImU32>(colorStyle.Selected),
    ImNodesCol_LinkHovered, static_cast<ImU32>(colorStyle.Hovered)
  };

  ImNodes::Link(linkId, startAttributeId, endAttributeId);

}

auto Link::WhichLinkCreated() noexcept -> std::optional<std::pair<int, int>> {
  int startAttrId = 0;
  int endAttrId = 0;

  if (!ImNodes::IsLinkCreated(&startAttrId, &endAttrId)) {
    return std::nullopt;
  }

  return std::make_pair(startAttrId, endAttrId);
}

} // namespace imsql::ui::nodes
