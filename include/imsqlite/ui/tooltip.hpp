#ifndef IMSQLITE_UI_TOOLTIP_HPP
#define IMSQLITE_UI_TOOLTIP_HPP

#include "imsqlite/ui/component.hpp"
#include "imgui.h"

namespace imsql::ui {

class TextTooltip final : public Component<"Tooltip"> {
public:
  explicit TextTooltip(RenderCtx& renderCtx, const char* text)
      : Component<"Tooltip">(renderCtx) {
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg, hicpp-vararg)
    ImGui::SetTooltip("%s", text);
  }
};

} // namespace imsql::ui

#endif // IMSQLITE_UI_TOOLTIP_HPP
