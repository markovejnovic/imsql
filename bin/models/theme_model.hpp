#ifndef IMSQL_THEME_MODEL_HPP
#define IMSQL_THEME_MODEL_HPP

#include "immm/base_types.hpp"

namespace imsql {

struct ThemeModel {
  struct {
    struct {
      struct {
        immm::Color Default = {"#2b4f82"};
        immm::Color Selected = {"#3369ad"};
        immm::Color Hover = {"#397ccd"};
      } TitleBar;
    } Database;
    struct {
      struct {
        immm::Color Default = {"#ee8434"};
        immm::Color Selected = {"#f99253"};
        immm::Color Hover = {"#ffa46b"};
      } TitleBar;
    } Operator;
    struct {
      struct {
        immm::Color Default = {"#1D6F42"};
        immm::Color Selected = {"#227a49"};
        immm::Color Hover = {"#29a561"};
      } TitleBar;
    } Spreadsheet;
  } Nodes;
};

} // namespace imsql

#endif // IMSQL_THEME_MODEL_HPP
