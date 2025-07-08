#ifndef IMSQLITE_OS_PLATFORM_PROVIDER_HPP
#define IMSQLITE_OS_PLATFORM_PROVIDER_HPP
#pragma once

#include <string>
#include <utility>

namespace imsql::os {

template <class Impl, class FrameHandleImpl>
class PlatformProvider {
public:
  /**
   * @brief Attempt to create a frame for the given platform.
   *
   * This function is allowed to throw children of WindowInitializationError.
   */
  auto CreateFrame(
    const std::string& title,
    std::pair<std::size_t, std::size_t> dimensions
  ) -> FrameHandleImpl {
    return static_cast<Impl*>(this)->CreateFrameImpl(std::move(title),
                                                     std::move(dimensions));
  }

private:
  PlatformProvider() = default;
  friend Impl;
};

} // namespace imsql::os

#endif // IMSQLITE_OS_PLATFORM_PROVIDER_HPP
