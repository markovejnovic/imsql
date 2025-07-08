#include "imsqlite/ui/tab_bar.hpp"
#include "imgui.h"
#include "imsqlite/ui/render_ctx.hpp"

namespace imsql::ui {

TabBar::TabBar(RenderCtx& renderCtx, const char* tabBarName)
    : Component(renderCtx), isRendering_(ImGui::BeginTabBar(tabBarName)) {}

TabBar::TabBar(RenderCtx& renderCtx, const std::string& tabBarName)
    : TabBar(renderCtx, tabBarName.c_str()) {}

TabBar::~TabBar() {
  if (isRendering_) {
    // End the tab bar only if it was successfully started.
    // This is important to avoid ImGui errors.
    ImGui::EndTabBar();
  }
}

} // namespace imsql::ui
