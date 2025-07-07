#ifndef IMSQLITE_MODEL_SHEET_HPP
#define IMSQLITE_MODEL_SHEET_HPP

#include "imsqlite/db/db.hpp"
#include <initializer_list>
#include <span>
#include <string>
#include <variant>
#include <vector>

namespace imsql::model {

template <class T, class ValueType>
class SheetCell {
public:
  [[nodiscard]] constexpr auto AsString() const -> std::string {
    return static_cast<const T*>(this)->AsStringImpl();
  }

private:
  SheetCell() = default;
  friend T;
};

// TODO(marko): this affinity is not correct -- the DB engine is slightly
// clever about how it reconstructs the type.
//class SheetCellNumeric final : public SheetCell<SheetCellNumeric, double> {
//public:
//  explicit constexpr SheetCellNumeric(double value) : value_(value) {}
//
//  [[nodiscard]] constexpr auto AsStringImpl() const -> std::string {
//    return std::to_string(value_);
//  }
//
//private:
//  double value_;
//};

class SheetCellInteger final : public SheetCell<SheetCellInteger, int64_t> {
public:
  explicit constexpr SheetCellInteger(int64_t value) : value_(value) {}

  [[nodiscard]] constexpr auto AsStringImpl() const -> std::string {
    return std::to_string(value_);
  }

private:
  int64_t value_;
};

class SheetCellText final : public SheetCell<SheetCellText, int64_t> {
public:
  explicit constexpr SheetCellText(std::string value)
    : value_(std::move(value)) {}

  [[nodiscard]] constexpr auto AsStringImpl() const -> std::string {
    return value_;
  }

private:
  std::string value_;
};

class SheetCellReal final : public SheetCell<SheetCellReal, double> {
public:
  explicit constexpr SheetCellReal(double value) : value_(value) {}

  [[nodiscard]] constexpr auto AsStringImpl() const -> std::string {
    return std::to_string(value_);
  }

private:
  double value_;
};

class SheetCellBlob final
  : public SheetCell<SheetCellBlob, std::vector<std::uint8_t>> {
public:
  explicit constexpr SheetCellBlob(std::string value)
    : value_(std::move(value)) {}

  [[nodiscard]] constexpr auto AsStringImpl() const -> std::string {
    return value_;
  }

private:
  std::string value_;
};

class SheetRow {
public:
  explicit SheetRow(
    std::vector<std::variant<SheetCellInteger,
                             SheetCellText,
                             SheetCellReal,
                             SheetCellBlob>> cells)
    : cells_(std::move(cells)) {}

  [[nodiscard]] constexpr auto Cells(
  ) const noexcept -> std::span<const std::variant<SheetCellInteger,
                                                   SheetCellText,
                                                   SheetCellReal,
                                                   SheetCellBlob>> {
    return {cells_};
  }

private:
  std::vector<std::variant<SheetCellInteger,
                           SheetCellText,
                           SheetCellReal,
                           SheetCellBlob>> cells_;
};

struct Column {
  explicit constexpr Column(std::string name)
    : name_(std::move(name)) {}

  [[nodiscard]] constexpr auto CName() const -> const std::string& {
    return name_;
  }

  [[nodiscard]] constexpr auto Name() -> std::string& {
    return name_;
  }

private:
  std::string name_;
};

class Spreadsheet {
public:
  Spreadsheet(std::initializer_list<Column> columns)
    : columns_(columns) {}

  constexpr void NewColumn() {
    columns_.emplace_back("New Column");
  }

  [[nodiscard]] constexpr auto CColumns(
  ) const noexcept -> std::span<const Column> {
    return {columns_};
  }

  [[nodiscard]] constexpr auto Columns(
  ) noexcept -> std::span<Column> {
    return {columns_};
  }

private:
  std::vector<Column> columns_;
};

class Sheet {
public:
  explicit Sheet(
    Spreadsheet spreadsheet,
    std::vector<SheetRow> rows,
    DbState dbState
  )
    : spreadsheet_(std::move(spreadsheet))
    , rows_(std::move(rows))
    , dbState_(std::move(dbState)) {}

  [[nodiscard]] auto Title() const -> std::string {
    return dbState_.Path();
  }

  [[nodiscard]] constexpr auto Rows(
  ) const -> std::span<const SheetRow> {
    return {rows_};
  }

  [[nodiscard]] constexpr auto CDb() const noexcept -> const DbState& {
    return dbState_;
  }

  [[nodiscard]] constexpr auto Spreadsheet() noexcept -> Spreadsheet& {
    return spreadsheet_;
  }

  [[nodiscard]] constexpr auto CSpreadSheet(
  ) const noexcept -> const class Spreadsheet& {
    return spreadsheet_;
  }

private:
  class Spreadsheet spreadsheet_;
  std::vector<SheetRow> rows_;
  DbState dbState_;
};

} // namespace imsql::model

#endif // IMSQLITE_MODEL_SHEET_HPP
