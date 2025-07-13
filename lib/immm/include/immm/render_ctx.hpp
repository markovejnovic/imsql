#ifndef IMMM_RENDER_CONTEXT_HPP
#define IMMM_RENDER_CONTEXT_HPP

#include "immm/detail/pch.hpp"

namespace immm {

class RenderCtx final {
 public:
  static constexpr bool kIsDebugEnabled = false;

  [[nodiscard]] constexpr auto RenderDepth() const noexcept -> std::size_t { return renderDepth_; }

  void Enter() noexcept { ++renderDepth_; }

  void Exit() noexcept {
    if (renderDepth_ > 0) {
      --renderDepth_;
    }
  }

  [[nodiscard]] constexpr auto IsFirstPaint() const noexcept -> bool { return firstPaint_; }

  void RenderCycle() { firstPaint_ = false; }

  [[nodiscard]] auto DbgStream() const -> std::ostream&;

 private:
  std::size_t renderDepth_ = 0;
  bool firstPaint_ = true;
};

}  // namespace immm

#endif  // IMMM_RENDER_CONTEXT_HPP
