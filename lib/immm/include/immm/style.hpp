#ifndef IMMM_STYLE_HPP
#define IMMM_STYLE_HPP

#include "immm/detail/pch.hpp"

namespace immm {

template <class... StyleAttr>
struct Style {
  static_assert(sizeof...(StyleAttr) % 2 == 0,
                "Style requires an even number of arguments, "
                "as they are expected to be key-value pairs.");

  explicit Style(StyleAttr&&... styleAttrs) { PushVar(std::forward<StyleAttr>(styleAttrs)...); }

  ~Style() { ImGui::PopStyleVar(sizeof...(StyleAttr) / 2); }

  Style(const Style&) = delete;
  auto operator=(const Style&) -> Style& = delete;
  Style(Style&&) = delete;
  auto operator=(Style&&) -> Style& = delete;

 private:
  static void PushVar() {}

  template <class K, class V, class... Rest>
  static void PushVar(K&& key, V&& value, Rest&&... rest) {
    ImGui::PushStyleVar(std::forward<K>(key), std::forward<V>(value));
    PushVar(std::forward<Rest>(rest)...);
  }
};

template <class... StyleAttr>
auto MakeStyle(StyleAttr&&... styleAttrs) -> Style<StyleAttr...> {
  return Style<StyleAttr...>(std::forward<StyleAttr>(styleAttrs)...);
}

}  // namespace immm

#endif  // IMMM_STYLE_HPP
