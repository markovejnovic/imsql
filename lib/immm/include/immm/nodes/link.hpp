#ifndef IMMM_NODES_LINK_HPP
#define IMMM_NODES_LINK_HPP

#include "immm/base_types.hpp"
#include "immm/component.hpp"
#include "immm/detail/pch.hpp"
#include "immm/render_ctx.hpp"

namespace immm::nodes {

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

}  // namespace immm::nodes

#endif  // IMMM_NODES_LINK_HPP
