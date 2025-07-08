#ifndef IMSQLITE_UI_WINDOW_HPP
#define IMSQLITE_UI_WINDOW_HPP

#include "imsqlite/ui/component.hpp"
#include "imsqlite/ui/render_ctx.hpp"

namespace imsql::ui {

class Window : public Component<"Window"> {
public:
  explicit Window(RenderCtx& renderCtx, const char* title);
  explicit Window(RenderCtx& renderCtx, const std::string& title);
  ~Window();

  [[nodiscard]] constexpr auto Rendering() const -> bool {
    return isRendering_;
  }

  Window(const Window&) = delete;
  auto operator=(const Window&) -> Window& = delete;
  Window(Window&&) = delete;
  auto operator=(Window&&) -> Window& = delete;

private:
  bool isRendering_;
};

} // namespace imsql::ui

#endif // IMSQLITE_UI_WINDOW_HPP
