#ifndef IMSQLITE_UI_BASE_TYPES_HPP
#define IMSQLITE_UI_BASE_TYPES_HPP

#include "imsqlite/pch/std.hpp"
#include "imsqlite/pch/imgui.hpp"
#include <charconv>
#include <limits>

namespace imsql::ui {

struct Vec2 {
  // NOLINTNEXTLINE(bugprone-easily-swappable-parameters, readability-identifier-length)
  explicit constexpr Vec2(float x = 0.0F, float y = 0.0F) noexcept
    : x_(x), y_(y) {}

  [[nodiscard]] constexpr auto X() const noexcept -> float {
    return x_;
  }

  [[nodiscard]] constexpr auto Y() const noexcept -> float {
    return y_;
  }

  [[nodiscard]] constexpr auto operator<=>(
    const Vec2& other
  ) const noexcept -> std::strong_ordering {
    const auto this_len = Length();
    const auto other_len = other.Length();

    if (this_len < other_len) {
      return std::strong_ordering::less;
    }
    if (this_len > other_len) {
      return std::strong_ordering::greater;
    }

    if (x_ == other.x_ && y_ == other.y_) {
      return std::strong_ordering::equal;
    }

    return std::strong_ordering::less;
  }

  [[nodiscard]] constexpr auto Length() const noexcept -> float {
    return std::sqrt((x_ * x_) + (y_ * y_));
  }

  [[nodiscard]] constexpr auto Normalized() const noexcept -> Vec2 {
    const auto length = Length();
    if (length == 0.0F) {
      return Vec2(0.0F, 0.0F);
    }

    return Vec2(x_ / length, y_ / length);
  }

private:
  float x_;
  float y_;

  template <class Archive> friend void serialize(Archive&, Vec2&, unsigned int);
};

/// @note I don't understand why ImGui uses these weird non-struct types for color representation,
///       etc. It's painful and not type-safe, so I am using a struct here instead.
///       The performance is literally the same, considering that they're literally the same size.
struct Color {
  // Note the ordering here matters since you need to make sure this is the same ordering as
  // IM_COL32, otherwise you risk paying some performance to convert the objects.
  //
  // TODO(marko): Verify the claim that this is indeed triviall convertible to/between ImU32
  // (without any instructions emitted).
  std::uint8_t Alpha = std::numeric_limits<unsigned char>::max();
  std::uint8_t Blue = 0;
  std::uint8_t Green = 0;
  std::uint8_t Red = 0;

  constexpr explicit Color() noexcept = default;

  // NOLINTBEGIN(bugprone-easily-swappable-parameters)
  constexpr Color(
    std::uint8_t red,
    std::uint8_t green,
    std::uint8_t blue,
    std::uint8_t alpha = std::numeric_limits<std::uint8_t>::max()
  ) noexcept
      : Alpha(alpha), Blue(blue), Green(green), Red(red) {}
  // NOLINTEND(bugprone-easily-swappable-parameters)

  // Boy I love everything being constexpr.
  consteval Color(std::string_view hexColor)
      : Alpha(std::numeric_limits<std::uint8_t>::max()) {
    if (hexColor[0] != '#') {
      throw std::invalid_argument("Hex color must start with '#'");
    }

    // Perform character conersion from hex to decimal.
    // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
    std::from_chars(hexColor.data() + 1, hexColor.data() + 3, Red, 16);
    std::from_chars(hexColor.data() + 3, hexColor.data() + 5, Green, 16);
    std::from_chars(hexColor.data() + 5, hexColor.data() + 7, Blue, 16);
    // NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
  }

  [[nodiscard]] constexpr auto operator==(const Color& other) const noexcept {
    return Red == other.Red && Green == other.Green && Blue == other.Blue && Alpha == other.Alpha;
  }

  [[nodiscard]] constexpr auto operator!=(const Color& other) const noexcept -> bool {
    return !(*this == other);
  }

  // NOLINTNEXTLINE(hicpp-explicit-conversions, google-explicit-constructor)
  [[nodiscard]] constexpr operator ImVec4() const noexcept {
    return {
      static_cast<float>(Red) / static_cast<float>(std::numeric_limits<std::uint8_t>::max()),
      static_cast<float>(Green) / static_cast<float>(std::numeric_limits<std::uint8_t>::max()),
      static_cast<float>(Blue) / static_cast<float>(std::numeric_limits<std::uint8_t>::max()),
      static_cast<float>(Alpha) / static_cast<float>(std::numeric_limits<std::uint8_t>::max())
    };
  }

  // NOLINTNEXTLINE(hicpp-explicit-conversions, google-explicit-constructor)
  [[nodiscard]] constexpr operator ImU32() const noexcept {
    return IM_COL32(Red, Green, Blue, Alpha);
  }
};

inline auto to_string(const Color& color) -> std::string {
  return std::format("#{:02x}{:02x}{:02x}{:02x}", color.Red, color.Green, color.Blue, color.Alpha);
}

} // namespace imsql::ui

#endif // IMSQLITE_UI_BASE_TYPES_HPP
