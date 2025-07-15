#include "immm/nodes/link.hpp"
#include "immm/nodes/color_style.hpp"

namespace immm::nodes {

Link::Link(RenderCtx& ctx, int linkId, int startAttributeId, int endAttributeId) : Component(ctx) {
  ImNodes::Link(linkId, startAttributeId, endAttributeId);
}

Link::Link(RenderCtx& ctx, int linkId, int startAttributeId, int endAttributeId,
           LinkColorStyle colorStyle)
    : Component(ctx) {
  const ColorStyle color_style{ImNodesCol_Link,         static_cast<ImU32>(colorStyle.Normal),
                               ImNodesCol_LinkSelected, static_cast<ImU32>(colorStyle.Selected),
                               ImNodesCol_LinkHovered,  static_cast<ImU32>(colorStyle.Hovered)};

  ImNodes::Link(linkId, startAttributeId, endAttributeId);
}

auto Link::WhichLinkCreated() noexcept -> std::optional<std::pair<int, int>> {
  int start_attr_id = 0;
  int end_attr_id = 0;

  if (!ImNodes::IsLinkCreated(&start_attr_id, &end_attr_id)) {
    return std::nullopt;
  }

  return std::make_pair(start_attr_id, end_attr_id);
}

auto Link::WhichLinkDestroyed() noexcept -> std::optional<int> {
  int link_id = 0;

  if (!ImNodes::IsLinkDestroyed(&link_id)) {
    return std::nullopt;
  }

  return link_id;
}

}  // namespace immm::nodes
