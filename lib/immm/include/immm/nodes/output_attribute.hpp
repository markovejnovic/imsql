#ifndef IMMM_NODES_OUTPUT_ATTRIBUTE_HPP
#define IMMM_NODES_OUTPUT_ATTRIBUTE_HPP

#include "immm/component.hpp"
#include "immm/render_ctx.hpp"

namespace immm::nodes {

class OutputAttribute final : public Component<"nodes::OutputAttribute"> {
 public:
  explicit OutputAttribute(RenderCtx& renderCtx, int attributeId);
  ~OutputAttribute();

  OutputAttribute(const OutputAttribute&) = delete;
  auto operator=(const OutputAttribute&) -> OutputAttribute& = delete;
  OutputAttribute(OutputAttribute&&) = delete;
  auto operator=(OutputAttribute&&) -> OutputAttribute& = delete;
};

}  // namespace immm::nodes

#endif  // IMMM_NODES_OUTPUT_ATTRIBUTE_HPP
