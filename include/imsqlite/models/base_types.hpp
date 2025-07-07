#ifndef IMSQLITE_MODELS_TABLE_HPP
#define IMSQLITE_MODELS_TABLE_HPP

#include <cstdlib>
#include <string>
#include <vector>

namespace imsql::models {

namespace detail {

struct UniqueId {
  explicit constexpr UniqueId(std::size_t underlyingId)
    : underlyingId_(underlyingId) {}

  [[nodiscard]] constexpr auto operator()() const noexcept -> std::size_t {
    return underlyingId_;
  }

private:
  std::size_t underlyingId_;
};

} // namespace detail

struct ColumnId final : public detail::UniqueId {
  explicit constexpr ColumnId(std::size_t underlyingId)
    : UniqueId(underlyingId) {}
};

struct TableId final : public detail::UniqueId {
  explicit constexpr TableId(std::size_t underlyingId)
    : UniqueId(underlyingId) {}
};

} // namespace imsql::models

#endif // IMSQLITE_MODELS_TABLE_HPP
