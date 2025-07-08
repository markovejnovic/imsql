#ifndef IMSQLITE_MODELS_DETAIL_UNIQUE_ID_HPP
#define IMSQLITE_MODELS_DETAIL_UNIQUE_ID_HPP

#include <compare>
#include <cstdlib>

namespace imsql::models::detail {

struct UniqueId {
  explicit constexpr UniqueId() noexcept
    : underlyingId_(0) {}

  explicit constexpr UniqueId(std::size_t underlyingId)
    : underlyingId_(underlyingId) {}

  [[nodiscard]] constexpr explicit operator std::size_t() const noexcept {
    return underlyingId_;
  }

  [[nodiscard]] constexpr auto operator<=>(const UniqueId&) const -> std::strong_ordering = default;

protected:
  [[nodiscard]] constexpr auto underlyingId() noexcept -> std::size_t& {
    return underlyingId_;
  }

private:
  std::size_t underlyingId_;
};

} // namespace imsql::models::detail

#endif // IMSQLITE_MODELS_DETAIL_UNIQUE_ID_HPP
