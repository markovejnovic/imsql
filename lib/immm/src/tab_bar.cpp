#include "immm/tab_bar.hpp"
#include "immm/detail/pch.hpp"
#include "immm/render_ctx.hpp"

namespace immm {

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

}  // namespace immm
