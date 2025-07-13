#ifndef IMMM_NODES_INPUT_ATTRIBUTE_HPP
#define IMMM_NODES_INPUT_ATTRIBUTE_HPP

#include "immm/component.hpp"
#include "immm/render_ctx.hpp"

namespace immm::nodes {

class InputAttribute final : public Component<"nodes::InputAttribute"> {
 public:
  explicit InputAttribute(RenderCtx& renderCtx, int attributeId);
  ~InputAttribute();

  InputAttribute(const InputAttribute&) = delete;
  auto operator=(const InputAttribute&) -> InputAttribute& = delete;
  InputAttribute(InputAttribute&&) = delete;
  auto operator=(InputAttribute&&) -> InputAttribute& = delete;
};

}  // namespace immm::nodes

#endif  // IMMM_NODES_INPUT_ATTRIBUTE_HPP
