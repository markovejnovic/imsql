#ifndef IMSQLITE_UI_NODES_LINK_HPP
#define IMSQLITE_UI_NODES_LINK_HPP

#include "imsqlite/ui/base_types.hpp"
#include "imsqlite/ui/component.hpp"
#include "imsqlite/ui/render_ctx.hpp"
#include "imsqlite/std.hpp"

namespace imsql::ui::nodes {

struct LinkColorStyle {
  Color Normal;
  Color Selected;
  Color Hovered;
};

class Link final : Component<"nodes::Link"> {
public:
  explicit Link(RenderCtx& ctx, int linkId, int startAttributeId, int endAttributeId);
  explicit Link(RenderCtx& ctx, int linkId, int startAttributeId, int endAttributeId,
                LinkColorStyle colorStyle);
  ~Link() = default;

  Link(const Link&) = delete;
  auto operator=(const Link&) -> Link& = delete;
  Link(Link&&) = delete;
  auto operator=(Link&&) -> Link& = delete;

  /// @brief Test whether the user requested a link between two attributes.
  static auto WhichLinkCreated() noexcept -> std::optional<std::pair<int, int>>;
};

} // namespace imsql::ui::nodes

#endif // IMSQLITE_UI_NODES_LINK_HPP
