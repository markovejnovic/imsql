#ifndef IMSQLITE_SQL_TABLE_HPP
#define IMSQLITE_SQL_TABLE_HPP
#pragma once

#include <algorithm>
#include <string>
#include "imsqlite/ex/str.hpp"

namespace imsql::sql {

class Table {
public:
  class Column {
  public:
    enum class Affinity : std::uint8_t {
      Text,
      Numeric,
      Integer,
      Real,
      Blob,
    };

    [[nodiscard]] static constexpr auto AffinityOf(
      const std::string& declaredType
    ) {
      if (ex::str::Contains(declaredType, "INT")) {
        return Affinity::Integer;
      }

      if (ex::str::Contains(declaredType, "CHAR") ||
          ex::str::Contains(declaredType, "CLOB") ||
          ex::str::Contains(declaredType, "TEXT")) {
        return Affinity::Text;
      }

      if (ex::str::Contains(declaredType, "REAL") ||
          ex::str::Contains(declaredType, "FLOA") ||
          ex::str::Contains(declaredType, "DOUB")) {
        return Affinity::Real;
      }

      if (ex::str::Contains(declaredType, "BLOB") ||
          declaredType == "") {
        return Affinity::Blob;
      }

      return Affinity::Numeric;
    }

    Column(std::string name, std::string type)
        : name_(std::move(name))
        , type_(std::move(type))
        , affinity_(AffinityOf(type_)) {}

    [[nodiscard]] constexpr auto Name() const noexcept -> const std::string& {
      return name_;
    }

    [[nodiscard]] constexpr auto Type() const noexcept -> const std::string& {
      return type_;
    }

    [[nodiscard]] constexpr auto Affinity() const noexcept -> Affinity {
      return affinity_;
    }

  private:
    std::string name_;
    std::string type_;
    enum Affinity affinity_;
  };

  explicit Table(std::string name) : name_(std::move(name)) {};

  [[nodiscard]] constexpr auto Name() const noexcept -> const std::string& {
    return name_;
  }

private:
  std::string name_;
};

} // namespace imsql::sql

#endif // IMSQLITE_SQL_TABLE_HPP
