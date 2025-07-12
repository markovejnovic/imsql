#ifndef IMSQLITE_UI_RENDER_CONTEXT_HPP
#define IMSQLITE_UI_RENDER_CONTEXT_HPP

#include "imsqlite/controllers/app_controllers.hpp"
#include "imsqlite/pch/std.hpp"

namespace imsql::ui {

class RenderCtx final {
public:
  static constexpr bool IsDebugEnabled = false;

  explicit RenderCtx(controllers::AppControllers&& controllers) noexcept
      : controllers_(std::move(controllers)) {}

  [[nodiscard]] constexpr auto RenderDepth() const noexcept -> std::size_t {
    return renderDepth_;
  }

  [[nodiscard]] constexpr auto Controllers() noexcept -> controllers::AppControllers& {
    return controllers_;
  }

  void Enter() noexcept {
    ++renderDepth_;
  }

  void Exit() noexcept {
    if (renderDepth_ > 0) {
      --renderDepth_;
    }
  }

  [[nodiscard]] constexpr auto IsFirstPaint() const noexcept -> bool {
    return firstPaint_;
  }

  void RenderCycle() {
    firstPaint_ = false;
  }

  [[nodiscard]] auto DbgStream() const noexcept -> std::ostream&;

  RenderCtx(const RenderCtx&) = delete;
  auto operator=(const RenderCtx&) -> RenderCtx& = delete;
  RenderCtx(RenderCtx&&) = default;
  auto operator=(RenderCtx&&) -> RenderCtx& = default;
  ~RenderCtx() = default;

private:
  std::size_t renderDepth_ = 0;
  bool firstPaint_ = true;

  controllers::AppControllers controllers_;
};

} // namespace imsql::ui

#endif // IMSQLITE_UI_RENDER_CONTEXT_HPP
