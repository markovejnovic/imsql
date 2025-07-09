#ifndef IMSQLITE_UI_NODES_LINK_HPP
#define IMSQLITE_UI_NODES_LINK_HPP

#include "imsqlite/ui/component.hpp"
#include "imsqlite/ui/render_ctx.hpp"

namespace imsql::ui::nodes {

class Link final : Component<"nodes::Link"> {
public:
  explicit Link(RenderCtx& ctx, int linkId, int startAttributeId, int endAttributeId);
  ~Link() = default;

  Link(const Link&) = delete;
  auto operator=(const Link&) -> Link& = delete;
  Link(Link&&) = delete;
  auto operator=(Link&&) -> Link& = delete;
};

} // namespace imsql::ui::nodes

#endif // IMSQLITE_UI_NODES_LINK_HPP
