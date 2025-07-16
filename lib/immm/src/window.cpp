#include "immm/window.hpp"
#include "immm/detail/pch.hpp"

namespace immm {

Window::Window(RenderCtx& renderCtx, const char* title)
    : Component(renderCtx), isRendering_(ImGui::Begin(title)) {}

Window::Window(RenderCtx& renderCtx, const std::string& title) : Window(renderCtx, title.c_str()) {}

auto Window::CreateCoveringWindow(RenderCtx& renderCtx, const char* title) -> Window {
  ImGui::SetNextWindowPos(ImVec2(0.0F, 0.0F), ImGuiCond_Always);
  ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);
  return Window(renderCtx, title);
}

Window::~Window() {
  if (isRendering_) {
    // End the window only if it was successfully started.
    // This is important to avoid ImGui errors.
    ImGui::End();
  }
}

}  // namespace immm
