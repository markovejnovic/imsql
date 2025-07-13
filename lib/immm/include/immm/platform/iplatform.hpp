#ifndef IMMM_PLATFORM_HPP
#define IMMM_PLATFORM_HPP

#include "immm/detail/pch.hpp"

namespace immm {

template <class Impl, class FrameHandleImpl>
class IPlatform {
 public:
  /**
   * @brief Attempt to create a frame for the given platform.
   *
   * This function is allowed to throw children of WindowInitializationError.
   */
  auto CreateFrame(const std::string& title, std::pair<std::size_t, std::size_t> dimensions)
      -> FrameHandleImpl {
    return static_cast<Impl*>(this)->CreateFrameImpl(std::move(title), std::move(dimensions));
  }

  auto CreateFrame(const std::string& title) -> FrameHandleImpl {
    return static_cast<Impl*>(this)->CreateFrameImpl(std::move(title));
  }

 private:
  IPlatform() = default;
  friend Impl;
};

}  // namespace immm

#endif  // IMMM_PLATFORM_HPP
