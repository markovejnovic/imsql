#ifndef IMMM_FRAME_HANDLE_HPP
#define IMMM_FRAME_HANDLE_HPP

#include "immm/detail/pch.hpp"

namespace immm {

struct RenderInfo {
  explicit RenderInfo(std::chrono::microseconds frameTime) : frameTime_(frameTime) {}

  [[nodiscard]] auto FrameTime() const noexcept -> std::chrono::microseconds { return frameTime_; }

 private:
  std::chrono::microseconds frameTime_;
};

template <class Impl>
class FrameHandle {
 public:
  /**
   * @brief Begin the main render loop.
   */
  void StartRendering(std::function<void(const RenderInfo&)>&& imRender,
                      std::function<void()>&& onQuit) {
    static_cast<Impl*>(this)->StartRenderingImpl(std::move(imRender), std::move(onQuit));
  }

 private:
  FrameHandle() = default;
  friend Impl;
};

}  // namespace immm

#endif  // IMMM_FRAME_HANDLE_HPP
