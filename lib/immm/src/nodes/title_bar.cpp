#include "immm/nodes/title_bar.hpp"
#include "immm/detail/pch.hpp"

namespace immm::nodes {

TitleBar::TitleBar(RenderCtx& renderCtx, const char* title) : Component(renderCtx) {
  ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(255, 0, 0, 255));
  ImNodes::BeginNodeTitleBar();
  ImGui::TextUnformatted(title);
}

TitleBar::TitleBar(RenderCtx& renderCtx, const std::string& title)
    : TitleBar(renderCtx, title.c_str()) {}

TitleBar::~TitleBar() {
  ImNodes::EndNodeTitleBar();
  ImNodes::PopColorStyle();
}

}  // namespace immm::nodes
