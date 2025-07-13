#include "immm/tab_item.hpp"
#include "immm/detail/pch.hpp"
#include "immm/render_ctx.hpp"

namespace immm {

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

}  // namespace immm
