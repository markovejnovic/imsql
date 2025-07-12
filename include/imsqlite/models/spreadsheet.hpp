#ifndef IMSQLITE_MODELS_SPREADSHEET_HPP
#define IMSQLITE_MODELS_SPREADSHEET_HPP

#include "imsqlite/models/base_types.hpp"
#include "imsqlite/pch/std.hpp"

namespace imsql::controllers {

class Spreadsheet;

} // namespace imsql::controllers

namespace imsql::models {

class Spreadsheet {
public:
  [[nodiscard]] constexpr auto CColumnName(const ColumnId& columnId) const noexcept -> std::string {
    return columnNames_.at(columnId);
  }

  [[nodiscard]] constexpr auto Columns() {
    return columnNames_ | std::views::keys;
  }

  [[nodiscard]] constexpr auto CColumns() const {
    return columnNames_ | std::views::keys;
  }

  [[nodiscard]] constexpr auto ColumnExists(ColumnId column) const noexcept -> bool {
    return columnNames_.contains(column);
  }

  friend class controllers::Spreadsheet;
  template <class Archive> friend void serialize(Archive&, Spreadsheet&, unsigned int);

private:
  boost::unordered_map<ColumnId, std::string> columnNames_;
};

} // namespace imsql::models

BOOST_CLASS_VERSION(imsql::models::Spreadsheet, 0);

#endif // IMSQLITE_MODELS_SPREADSHEET_HPP
