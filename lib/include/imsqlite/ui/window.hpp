#ifndef IMSQLITE_UI_WINDOW_HPP
#define IMSQLITE_UI_WINDOW_HPP
#pragma once

#include <string_view>

namespace imsql::ui {

class Window {
public:
  explicit Window(const char* title);
  ~Window();

  Window(const Window&) = delete;
  auto operator=(const Window&) -> Window& = delete;
  Window(Window&&) = delete;
  auto operator=(Window&&) -> Window& = delete;
};

} // namespace imsql::ui

#endif // IMSQLITE_UI_WINDOW_HPP
