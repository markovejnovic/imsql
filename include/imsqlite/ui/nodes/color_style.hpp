#ifndef IMSQLITE_UI_NODES_COLOR_STYLE_HPP
#define IMSQLITE_UI_NODES_COLOR_STYLE_HPP

#include "imnodes.h"
#include <utility>

namespace imsql::ui::nodes {

template <class... ColorStyleAttr>
struct ColorStyle {
  static_assert(sizeof...(ColorStyleAttr) % 2 == 0,
                "ColorStyle requires an even number of arguments, "
                "as they are expected to be key-value pairs.");

  explicit ColorStyle(ColorStyleAttr&&... styleAttrs) {
    PushVar(std::forward<ColorStyleAttr>(styleAttrs)...);
  }

  ~ColorStyle() {
    for (std::size_t i = 0; i < sizeof...(ColorStyleAttr) / 2; ++i) {
      // Pop the color style for each pair of key-value pushed.
      ImNodes::PopColorStyle();
    }
  }

  ColorStyle(const ColorStyle&) = delete;
  auto operator=(const ColorStyle&) -> ColorStyle& = delete;
  ColorStyle(ColorStyle&&) = delete;
  auto operator=(ColorStyle&&) -> ColorStyle& = delete;

private:
  static void PushVar() { }

  template <class K, class V, class... Rest>
  static void PushVar(K&& key, V&& value, Rest&&... rest) {
    ImNodes::PushColorStyle(std::forward<K>(key), std::forward<V>(value));
    PushVar(std::forward<Rest>(rest)...);
  }
};

template <class... ColorStyleAttr>
auto MakeColorStyle(ColorStyleAttr&&... styleAttrs) -> ColorStyle<ColorStyleAttr...> {
  return ColorStyle<ColorStyleAttr...>(std::forward<ColorStyleAttr>(styleAttrs)...);
}

} // namespace imsql::ui::nodes

#endif // IMSQLITE_UI_NODES_COLOR_STYLE_HPP
