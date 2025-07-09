#ifndef IMSQLITE_MATH_INTERVAL_HPP
#define IMSQLITE_MATH_INTERVAL_HPP

#include <utility>
namespace imsql::math {

template <class T>
class Interval {
public:
  static_assert(std::is_arithmetic_v<T>, "T must be an arithmetic type");
  enum class BoundType : uint8_t {
    Open,
    Closed,
  };

  explicit constexpr Interval(std::pair<T, T> range) noexcept
    : range_(std::move(range)) {}

  [[nodiscard]] constexpr auto Lower() const noexcept -> T {
    return range_.first;
  }

  [[nodiscard]] constexpr auto Upper() const noexcept -> T {
    return range_.second;
  }

  template <BoundType lowerBoundType = BoundType::Closed,
            BoundType upperBoundType = BoundType::Closed>
  [[nodiscard]] constexpr auto Contains(const T& value) const noexcept -> bool {
    if constexpr (lowerBoundType == BoundType::Open && upperBoundType == BoundType::Open) {
      return value > Lower() && value < Upper();
    }

    if constexpr (lowerBoundType == BoundType::Open && upperBoundType == BoundType::Closed) {
      return value > Lower() && value <= Upper();
    }

    if constexpr (lowerBoundType == BoundType::Closed && upperBoundType == BoundType::Open) {
      return value >= Lower() && value < Upper();
    }

    return value >= Lower() && value <= Upper();
  }

  [[nodiscard]] constexpr auto Below(const T& value) const noexcept -> Interval<T> {
    return Interval<T>(std::make_pair(Lower() - value, Lower()));
  }

  [[nodiscard]] constexpr auto Above(const T& value) const noexcept -> Interval<T> {
    return Interval<T>(std::make_pair(Upper(), Upper() + value));
  }

  [[nodiscard]] constexpr auto Length() const noexcept -> T {
    return Upper() - Lower();
  }

  [[nodiscard]] constexpr auto IsEmpty() const noexcept -> bool {
    return Lower() >= Upper();
  }

private:
  std::pair<T, T> range_;
};

template <class T>
[[nodiscard]] constexpr auto MakeInterval(std::pair<T, T> range) -> Interval<T> {
  return Interval<T>(std::move(range));
}

template <class T>
[[nodiscard]] constexpr auto MakeEndingInterval(const T& endingAt, const T& length) -> Interval<T> {
  return MakeInterval<T>(std::make_pair(endingAt - length, endingAt));
}

template <class T>
[[nodiscard]] constexpr auto MakeStartingInterval(
  const T& startingAt,
  const T& length
) -> Interval<T> {
  return MakeInterval<T>(std::make_pair(startingAt, startingAt + length));
}

} // namespace imsql::math

#endif // IMSQLITE_MATH_INTERVAL_HPP
