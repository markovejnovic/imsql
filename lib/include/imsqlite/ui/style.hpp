#ifndef IMSQLITE_UI_STYLE_HPP
#define IMSQLITE_UI_STYLE_HPP

#include "imgui.h"
#include "imnodes.h"
#include <utility>
namespace imsql::ui {

template <class... StyleAttr>
struct Style {
  static_assert(sizeof...(StyleAttr) % 2 == 0,
                "Style requires an even number of arguments, "
                "as they are expected to be key-value pairs.");

  explicit Style(StyleAttr&&... styleAttrs) {
    PushVar(std::forward<StyleAttr>(styleAttrs)...);
  }

  ~Style() {
    ImGui::PopStyleVar(sizeof...(StyleAttr) / 2);
  }

  Style(const Style&) = delete;
  auto operator=(const Style&) -> Style& = delete;
  Style(Style&&) = delete;
  auto operator=(Style&&) -> Style& = delete;

private:
  static void PushVar() { }

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

template <class... NodeColorStyleAttr>
struct NodeColorStyle {
  static_assert(sizeof...(NodeColorStyleAttr) % 2 == 0,
                "NodeColorStyle requires an even number of arguments, "
                "as they are expected to be key-value pairs.");

  explicit NodeColorStyle(NodeColorStyleAttr&&... styleAttrs) {
    PushVar(std::forward<NodeColorStyleAttr>(styleAttrs)...);
  }

  ~NodeColorStyle() {
    for (std::size_t i = 0; i < sizeof...(NodeColorStyleAttr) / 2; ++i) {
      // Pop the color style for each pair of key-value pushed.
      ImNodes::PopColorStyle();
    }
  }

  NodeColorStyle(const NodeColorStyle&) = delete;
  auto operator=(const NodeColorStyle&) -> NodeColorStyle& = delete;
  NodeColorStyle(NodeColorStyle&&) = delete;
  auto operator=(NodeColorStyle&&) -> NodeColorStyle& = delete;

private:
  static void PushVar() { }

  template <class K, class V, class... Rest>
  static void PushVar(K&& key, V&& value, Rest&&... rest) {
    ImNodes::PushColorStyle(std::forward<K>(key), std::forward<V>(value));
    PushVar(std::forward<Rest>(rest)...);
  }
};

template <class... NodeColorStyleAttr>
auto MakeNodeColorStyle(NodeColorStyleAttr&&... styleAttrs) -> NodeColorStyle<NodeColorStyleAttr...> {
  return NodeColorStyle<NodeColorStyleAttr...>(std::forward<NodeColorStyleAttr>(styleAttrs)...);
}

} // namespace imsql::ui

#endif // IMSQLITE_UI_STYLE_HPP
