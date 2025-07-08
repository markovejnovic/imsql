#ifndef IMSQLITE_UI_NODES_STATIC_ATTRIBUTE_HPP
#define IMSQLITE_UI_NODES_STATIC_ATTRIBUTE_HPP

#include "imsqlite/ui/component.hpp"
#include "imsqlite/ui/render_ctx.hpp"

namespace imsql::ui::nodes {

class StaticAttribute final : public Component<"nodes::StaticAttribute"> {
public:
  explicit StaticAttribute(RenderCtx& renderCtx, int attributeId);
  ~StaticAttribute();

  StaticAttribute(const StaticAttribute&) = delete;
  auto operator=(const StaticAttribute&) -> StaticAttribute& = delete;
  StaticAttribute(StaticAttribute&&) = delete;
  auto operator=(StaticAttribute&&) -> StaticAttribute& = delete;
};

} // namespace imsql::ui::nodes

#endif // IMSQLITE_UI_NODES_STATIC_ATTRIBUTE_HPP
