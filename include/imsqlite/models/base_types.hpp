#ifndef IMSQLITE_MODELS_TABLE_HPP
#define IMSQLITE_MODELS_TABLE_HPP

#include <string>
#include "detail/unique_id.hpp"

namespace imsql::models {

struct ColumnId final : public detail::UniqueId {
  explicit constexpr ColumnId() noexcept = default;

  explicit constexpr ColumnId(std::size_t underlyingId)
    : UniqueId(underlyingId) {}

  [[nodiscard]] constexpr auto operator==(const ColumnId& other) const -> bool {
    return static_cast<std::size_t>(*this) == static_cast<std::size_t>(other);
  }

  [[nodiscard]] constexpr auto operator!= (const ColumnId& other) const -> bool {
    return !(*this == other);
  }
};

auto hash_value(const ColumnId& columnId) -> std::size_t;

struct TableId final : public detail::UniqueId {
  explicit constexpr TableId() noexcept = default;

  explicit constexpr TableId(std::size_t underlyingId)
    : UniqueId(underlyingId) {}

  [[nodiscard]] constexpr auto operator==(const TableId& other) const -> bool {
    return static_cast<std::size_t>(*this) == static_cast<std::size_t>(other);
  }

  [[nodiscard]] constexpr auto operator!= (const TableId& other) const -> bool {
    return !(*this == other);
  }

  template <class Archive> friend void serialize(Archive&, TableId&, unsigned int);
};

auto hash_value(const TableId& columnId) -> std::size_t;

enum class OnDeleteOnUpdateAction : uint8_t {
  NoAction,
  Restrict,
  SetNull,
  SetDefault,
  Cascade,
};

inline auto OnDeleteOnUpdateActionFromString(
  const std::string& action
) -> OnDeleteOnUpdateAction {
  if (action == "NO ACTION") {
    return OnDeleteOnUpdateAction::NoAction;
  }
  if (action == "RESTRICT") {
    return OnDeleteOnUpdateAction::Restrict;
  }
  if (action == "SET NULL") {
    return OnDeleteOnUpdateAction::SetNull;
  }
  if (action == "SET DEFAULT") {
    return OnDeleteOnUpdateAction::SetDefault;
  }
  if (action == "CASCADE") {
    return OnDeleteOnUpdateAction::Cascade;
  }

  throw std::invalid_argument("Unknown action: " + action);
}

} // namespace imsql::models

#endif // IMSQLITE_MODELS_TABLE_HPP
