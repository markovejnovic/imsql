#ifndef IMSQLITE_OS_PLATFORM_FRAME_HANDLE_HPP
#define IMSQLITE_OS_PLATFORM_FRAME_HANDLE_HPP
#pragma once

#include <functional>
#include <chrono>

namespace imsql::os {

struct RenderInfo {
  explicit RenderInfo(std::chrono::microseconds frameTime)
      : frameTime_(frameTime) {}

  [[nodiscard]] auto FrameTime() const noexcept -> std::chrono::microseconds {
    return frameTime_;
  }

private:
  std::chrono::microseconds frameTime_;
};


template <class Impl>
class FrameHandle {
public:
  /**
   * @brief Begin the main render loop.
   */
  void StartRendering(std::function<void(const RenderInfo&)>&& imRender) {
    static_cast<Impl*>(this)->StartRenderingImpl(std::move(imRender));
  }

private:
  FrameHandle() = default;
  friend Impl;
};

} // namespace imsql::os

#endif // IMSQLITE_OS_PLATFORM_FRAME_HANDLE_HPP
