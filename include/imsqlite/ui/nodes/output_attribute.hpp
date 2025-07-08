#ifndef IMSQLITE_UI_NODES_OUTPUT_ATTRIBUTE_HPP
#define IMSQLITE_UI_NODES_OUTPUT_ATTRIBUTE_HPP

#include "imsqlite/ui/component.hpp"
#include "imsqlite/ui/render_ctx.hpp"

namespace imsql::ui::nodes {

class OutputAttribute final : public Component<"nodes::OutputAttribute"> {
public:
  explicit constexpr OutputAttribute(RenderCtx& renderCtx, int attributeId);
  constexpr ~OutputAttribute();

  OutputAttribute(const OutputAttribute&) = delete;
  auto operator=(const OutputAttribute&) -> OutputAttribute& = delete;
  OutputAttribute(OutputAttribute&&) = delete;
  auto operator=(OutputAttribute&&) -> OutputAttribute& = delete;
};

} // namespace imsql::ui::nodes

#endif // IMSQLITE_UI_NODES_OUTPUT_ATTRIBUTE_HPP
