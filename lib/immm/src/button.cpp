#include "immm/button.hpp"
#include "immm/detail/pch.hpp"

namespace immm {

auto Button::CreateAndIsClicked(RenderCtx& renderCtx, const char* label)
    -> std::pair<Button, bool> {
  return {Button(renderCtx), ImGui::Button(label)};
}

auto Button::CreateAndIsClicked(RenderCtx& renderCtx, const std::string& label)
    -> std::pair<Button, bool> {
  return CreateAndIsClicked(renderCtx, label.c_str());
}

Button::Button(RenderCtx& renderCtx) : Component(renderCtx) {}

}  // namespace immm
