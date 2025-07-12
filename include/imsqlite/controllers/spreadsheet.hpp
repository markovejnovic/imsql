#ifndef IMSQLITE_CONTROLLERS_SPREADSHEET_HPP
#define IMSQLITE_CONTROLLERS_SPREADSHEET_HPP

#include "imsqlite/controllers/identifier.hpp"
#include "imsqlite/models/spreadsheet.hpp"
#include "imsqlite/pch/std.hpp"

namespace imsql {

  auto Main(int /*argc*/, char *const *const /*argv*/) -> int;

} // namespace imsql

namespace imsql::controllers {

class Spreadsheet {
public:
  constexpr explicit Spreadsheet(std::shared_ptr<Identifier> identCtrl)
      : identCtrl_(std::move(identCtrl)) {}

  void AddNewColumn();

  [[nodiscard]] constexpr auto CModel() const noexcept -> const models::Spreadsheet& {
    return model_;
  }

private:
  static constexpr auto kDefaultColumnName = "Unnamed Column";

  std::shared_ptr<Identifier> identCtrl_;

  models::Spreadsheet model_;

  // TODO(marko): This is a hack, obviously
  friend auto imsql::Main(int /*argc*/, char *const *const /*argv*/) -> int;
};

} // namespace imsql::controllers

#endif // IMSQLITE_CONTROLLERS_SPREADSHEET_HPP
