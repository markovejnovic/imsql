#ifndef IMSQLITE_UI_NODES_INPUT_ATTRIBUTE_HPP
#define IMSQLITE_UI_NODES_INPUT_ATTRIBUTE_HPP

#include "imsqlite/ui/component.hpp"
#include "imsqlite/ui/render_ctx.hpp"

namespace imsql::ui::nodes {

class InputAttribute final : public Component<"nodes::InputAttribute"> {
public:
  explicit InputAttribute(RenderCtx& renderCtx, int attributeId);
  ~InputAttribute();

  InputAttribute(const InputAttribute&) = delete;
  auto operator=(const InputAttribute&) -> InputAttribute& = delete;
  InputAttribute(InputAttribute&&) = delete;
  auto operator=(InputAttribute&&) -> InputAttribute& = delete;
};

} // namespace imsql::ui::nodes

#endif // IMSQLITE_UI_NODES_INPUT_ATTRIBUTE_HPP
