#ifndef IMSQLITE_UI_NODES_BIDIRECTIONAL_ATTRIBUTE_HPP
#define IMSQLITE_UI_NODES_BIDIRECTIONAL_ATTRIBUTE_HPP

#include "imsqlite/ui/component.hpp"
#include "imsqlite/ui/nodes/input_attribute.hpp"
#include "imsqlite/ui/nodes/output_attribute.hpp"
#include "imsqlite/ui/render_ctx.hpp"

namespace imsql::ui::nodes {

class BidirectionalAttribute final : public Component<"nodes::BidirectionalAttribute"> {
public:
  template <class InputRenderer, class OutputRenderer>
  explicit BidirectionalAttribute(
    RenderCtx& renderCtx,
    int inputId,
    InputRenderer&& inputRenderer,
    int outputId,
    OutputRenderer&& outputRenderer
  ) : Component(renderCtx) {
    const ImVec2 node_pos = ImGui::GetCursorScreenPos();

    ImGui::SetCursorScreenPos(node_pos);
    {
      InputAttribute input_attr{renderCtx, inputId};
      std::invoke(std::forward<InputRenderer>(inputRenderer));
    }

    ImGui::SetCursorScreenPos(node_pos);
    {
      OutputAttribute output_attr{renderCtx, outputId};
      std::invoke(std::forward<OutputRenderer>(outputRenderer));
    }
  }

  BidirectionalAttribute(const BidirectionalAttribute&) = delete;
  auto operator=(const BidirectionalAttribute&) -> BidirectionalAttribute& = delete;
  BidirectionalAttribute(BidirectionalAttribute&&) = delete;
  auto operator=(BidirectionalAttribute&&) -> BidirectionalAttribute& = delete;
};

} // namespace imsql::ui::nodes

#endif // IMSQLITE_UI_NODES_BIDIRECTIONAL_ATTRIBUTE_HPP
