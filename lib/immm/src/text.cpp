#include "immm/text.hpp"
#include "immm/detail/pch.hpp"

namespace immm {

Text::Text(RenderCtx& renderCtx, const char* label) : Component(renderCtx) {
  ImGui::TextUnformatted(label);
}

Text::Text(RenderCtx& renderCtx, const std::string& label) : Text(renderCtx, label.c_str()) {}

Text::~Text() = default;

}  // namespace immm
