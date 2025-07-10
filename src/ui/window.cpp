#include "imsqlite/ui/window.hpp"
#include "imsqlite/pch/imgui.hpp"

namespace imsql::ui {

Window::Window(RenderCtx& renderCtx, const char* title)
    : Component(renderCtx), isRendering_(ImGui::Begin(title)) {}

Window::Window(RenderCtx& renderCtx, const std::string& title)
    : Window(renderCtx, title.c_str()) {}

Window::~Window() {
  if (isRendering_) {
    // End the window only if it was successfully started.
    // This is important to avoid ImGui errors.
    ImGui::End();
  }
}

} // namespace imsql::ui
