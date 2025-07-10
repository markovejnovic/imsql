#ifndef IMSQLITE_MODELS_THEME_HPP
#define IMSQLITE_MODELS_THEME_HPP

#include "imsqlite/ui/base_types.hpp"
namespace imsql::models {

struct Theme {
  ui::Color UserDrawnLinkColor = ui::Color("#ff0000");
  ui::Color UserDrawnLinkColorHover = ui::Color("#ff0000");
  ui::Color UserDrawnLinkColorSelected = ui::Color("#ff0000");
};

} // namespace imsql::models

#endif // IMSQLITE_MODELS_THEME_HPP
