#include "imsqlite/ui/input_text.hpp"
#include "imsqlite/ui/render_ctx.hpp"
#include "imsqlite/pch/imgui.hpp"
#include "gsl/gsl"

namespace imsql::ui {

constexpr InputText::InputText(
  RenderCtx& renderCtx,
  const char* label,
  std::string& value,
  InputTextOption options
) : Component(renderCtx) {
  if (options != InputTextOption::None) {
    // TODO(marko): Implement more flags.
    throw std::invalid_argument("Unsupported InputTextOption");
  }

  static constexpr std::size_t kInitialCapacity = 15;
  value.reserve(kInitialCapacity);

  const auto callback = [](ImGuiInputTextCallbackData* data) -> int {
      auto* str = static_cast<std::string*>(data->UserData);
      if (data->EventFlag == ImGuiInputTextFlags_CallbackResize) {
          str->resize(gsl::narrow<int>(data->BufTextLen * 2));
          data->Buf = str->data();
          data->BufSize = gsl::narrow<int>(str->capacity() + 1);
      }
      return 0;
  };

  ImGui::InputText(
      label,
      value.data(),
      value.capacity(),
      ImGuiInputTextFlags_CallbackResize,
      +callback,
      static_cast<void*>(&value)
  );
}

} // namespace imsql::ui
