#include "imgui.h"
#include "imsqlite/ui/window.hpp"

namespace imsql::ui {

Window::Window(const char* title) {
  ImGui::Begin(title);
}

Window::~Window() {
  ImGui::End();
}

} // namespace imsql::ui
