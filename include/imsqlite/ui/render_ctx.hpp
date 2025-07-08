#ifndef IMSQLITE_UI_RENDER_CONTEXT_HPP
#define IMSQLITE_UI_RENDER_CONTEXT_HPP

#include <cstdlib>
#include <iostream>
#include <ostream>

namespace imsql::ui {

class RenderCtx final {
public:
  static constexpr bool IsDebugEnabled = false;

  [[nodiscard]] constexpr auto RenderDepth() const noexcept -> std::size_t {
    return renderDepth_;
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

private:
  std::size_t renderDepth_ = 0;
  bool firstPaint_ = true;
};

} // namespace imsql::ui

#endif // IMSQLITE_UI_RENDER_CONTEXT_HPP
