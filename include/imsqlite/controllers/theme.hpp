#ifndef IMSQLITE_CONTROLLERS_THEME_HPP
#define IMSQLITE_CONTROLLERS_THEME_HPP

#include "imsqlite/models/theme.hpp"

namespace imsql::controllers {

class Theme {
public:
  [[nodiscard]] constexpr auto Model() const noexcept -> const models::Theme& {
    return theme_;
  }

private:
  models::Theme theme_;
};

} // namespace imsql::controllers

#endif // IMSQLITE_CONTROLLERS_THEME_HPP
