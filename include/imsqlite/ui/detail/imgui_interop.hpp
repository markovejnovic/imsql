#ifndef IMSQLITE_UI_DETAIL_IMGUI_INTEROP_HPP
#define IMSQLITE_UI_DETAIL_IMGUI_INTEROP_HPP

#include "imsqlite/pch/imgui.hpp"
#include "imgui_internal.h"
#include "imsqlite/ui/base_types.hpp"
#include <string>

namespace imsql::ui::detail {

[[nodiscard]] constexpr auto Vec2ToImVec2(const ui::Vec2& vec) noexcept -> ImVec2 {
  return ImVec2{vec.X(), vec.Y()};
}

[[nodiscard]] constexpr auto ImVec2ToVec2(const ImVec2& vec) noexcept -> ui::Vec2 {
  return ui::Vec2{vec.x, vec.y};
}

[[nodiscard]] constexpr auto Utf8Bytes(int value) -> std::string {
  std::string result;
  result.reserve(4);
  ImTextCharToUtf8(result.data(), value);
  return result;
}

} // namespace imsql::ui::detail

#endif // IMSQLITE_UI_DETAIL_IMGUI_INTEROP_HPP
