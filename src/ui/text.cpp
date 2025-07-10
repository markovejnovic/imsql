#include "imsqlite/ui/text.hpp"
#include "imsqlite/pch/imgui.hpp"


namespace imsql::ui {

Text::Text(RenderCtx& renderCtx, const char* label) : Component(renderCtx) {
  ImGui::TextUnformatted(label);
}

Text::Text(RenderCtx& renderCtx, const std::string& label)
    : Text(renderCtx, label.c_str()) {}

Text::~Text() = default;

} // namespace imsql::ui
