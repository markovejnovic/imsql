#ifndef IMMM_DETAIL_IMGUI_INTEROP_HPP
#define IMMM_DETAIL_IMGUI_INTEROP_HPP

#include "imgui_internal.h"
#include "immm/base_types.hpp"
#include "immm/detail/pch.hpp"

namespace immm::detail {

[[nodiscard]] constexpr auto Vec2ToImVec2(const Vec2& vec) noexcept -> ImVec2 {
  return ImVec2{vec.X(), vec.Y()};
}

[[nodiscard]] constexpr auto ImVec2ToVec2(const ImVec2& vec) noexcept -> Vec2 {
  return Vec2{vec.x, vec.y};
}

[[nodiscard]] constexpr auto Utf8Bytes(int value) -> std::string {
  std::string result;
  result.reserve(4);
  ImTextCharToUtf8(result.data(), value);
  return result;
}

}  // namespace immm::detail

#endif  // IMMM_DETAIL_IMGUI_INTEROP_HPP
