#include "imsqlite/ui/tab_item.hpp"
#include "imgui.h"
#include "imsqlite/ui/render_ctx.hpp"

namespace imsql::ui {

TabItem::TabItem(RenderCtx& renderCtx, const char* tabItemName)
    : Component(renderCtx), isRendering_(ImGui::BeginTabItem(tabItemName)) {}

TabItem::TabItem(RenderCtx& renderCtx, const std::string& tabItemName)
    : TabItem(renderCtx, tabItemName.c_str()) {}

TabItem::~TabItem() {
  if (isRendering_) {
    // End the tab bar only if it was successfully started.
    // This is important to avoid ImGui errors.
    ImGui::EndTabItem();
  }
}

} // namespace imsql::ui
