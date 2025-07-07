#ifndef IMSQLITE_CONTROLLER_SHEET_HPP
#define IMSQLITE_CONTROLLER_SHEET_HPP

#include <vector>
namespace imsql::controller {

class Sheet {
public:
  class Column {
  public:
    explicit Column(std::string name) : name_(std::move(name)) {}

    [[nodiscard]] auto Name() const -> const std::string& {
      return name_;
    }

  private:
    std::string name_;
  };

  class Row {
  };

  Sheet() = default;

private:
  std::vector<Column> columns_;
  std::vector<
};

} // namespace imsql::controller

#endif // IMSQLITE_CONTROLLER_SHEET_HPP
