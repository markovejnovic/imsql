#ifndef IMSQLITE_UI_DETAIL_COMPONENT_LABEL_HPP
#define IMSQLITE_UI_DETAIL_COMPONENT_LABEL_HPP

#include <cstdlib>

namespace imsql::ui::detail {

template <std::size_t N>
struct ComponentLabel {
  // NOLINTBEGIN(hicpp-avoid-c-arrays, cppcoreguidelines-avoid-c-arrays,
  //             modernize-avoid-c-arrays)
  constexpr ComponentLabel(const char (&str)[N]) {
    for (std::size_t i = 0; i < N; ++i) {
      value__[i] = str[i];
    }
  }

  [[nodiscard]] constexpr auto operator==(const ComponentLabel&) const -> bool = default;

  [[nodiscard]] constexpr auto String() const noexcept -> const char* {
    return value__;
  }

  char value__[N];
  // NOLINTEND
};

} // namespace imsql::ui::detail

#endif // IMSQLITE_UI_DETAIL_COMPONENT_LABEL_HPP
