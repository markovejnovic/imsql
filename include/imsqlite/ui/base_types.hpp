#ifndef IMSQLITE_UI_BASE_TYPES_HPP
#define IMSQLITE_UI_BASE_TYPES_HPP

#include <cmath>
#include <compare>

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

} // namespace imsql::ui

#endif // IMSQLITE_UI_BASE_TYPES_HPP
