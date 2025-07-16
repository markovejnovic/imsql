#ifndef IMSQL_MODELS_DG_NODES_VALUES_HPP
#define IMSQL_MODELS_DG_NODES_VALUES_HPP

#include "pch.hpp"
#include "util/fp.hpp"
#include <optional>
#include <utility>

namespace imsql::dg {

class Int64Value {
  public:
    explicit Int64Value(int64_t value) : value_(value) {}
  
    [[nodiscard]] auto Value() const noexcept -> int64_t {
      return value_;
    }
  
  private:
    int64_t value_;
};

class StringValue {
  public:
    explicit StringValue(std::string value) : value_(std::move(value)) {}
  
    [[nodiscard]] auto Value() const noexcept -> const std::string& {
      return value_;
    }
  
  private:
    std::string value_;
};

class NullValue {};

using Value = std::variant<Int64Value, StringValue, NullValue>;
enum class ValueTypeTag : uint8_t {
  // @todo(marko): Instead of using an enum, we should use a type trait to determine the index of
  // the variant.
  Int64 = 0,
  String = 1,
  Null = 2,
};

} // namespace imsql::dg

template <>
struct std::formatter<imsql::dg::Value> {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const imsql::dg::Value& value, FormatContext& ctx) const {
      return std::visit(imsql::util::Overloads {
        [&](const imsql::dg::NullValue&) { return std::format_to(ctx.out(), "[NULL]"); },
        [&](auto&& val) {
            return std::format_to(ctx.out(), "{}", val.Value());
        }
      }, value);
    }
};

#endif // IMSQL_MODELS_DG_NODES_VALUES_HPP
