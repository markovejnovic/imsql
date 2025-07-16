#ifndef IMMM_WINDOW_HPP
#define IMMM_WINDOW_HPP

#include "immm/component.hpp"
#include "immm/render_ctx.hpp"

namespace immm {

class Window : public Component<"Window"> {
 public:
  explicit Window(RenderCtx& renderCtx, const char* title);
  explicit Window(RenderCtx& renderCtx, const std::string& title);
  ~Window();

  [[nodiscard]] constexpr auto Rendering() const -> bool { return isRendering_; }

  Window(const Window&) = delete;
  auto operator=(const Window&) -> Window& = delete;
  Window(Window&&) = delete;
  auto operator=(Window&&) -> Window& = delete;

  static auto CreateCoveringWindow(RenderCtx& renderCtx, const char* title) -> Window;

  static constexpr auto CreateCoveringWindow(RenderCtx& renderCtx,
                                             const std::string& title) -> Window {
    return CreateCoveringWindow(renderCtx, title.c_str());
  }

 private:
  bool isRendering_;
};

}  // namespace immm

#endif  // IMMM_WINDOW_HPP
