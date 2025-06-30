#ifndef IMSQLITE_OS_PLATFORM_MACOS_HPP
#define IMSQLITE_OS_PLATFORM_MACOS_HPP
#include <functional>
#pragma once

#ifdef __APPLE__

#include "frame_handle.hpp"
#include "platform_provider.hpp"
#include <optional>
#include <type_traits>
#include "GLFW/glfw3.h"
#include "error.hpp"

namespace imsql::os {

struct GlfwError : public PlatformError {
  /**
   * @brief If this error is thrown, GLFW failed to create a window.
   * @see https://www.glfw.org/docs/3.3/group__errors.html#gaa0b8c1f2d4e5c6f7b9d8e4f0c1a2b5f6
   */
  GlfwError(int errCode, const char* description);

  /**
   * @brief Returns the current GLFW error, if any.
   * @{
   */
  static auto CurrentMaybe() -> std::optional<GlfwError>;
  static auto Current() -> GlfwError;
  /**
   * @}
   */
  /**
   * @brief Flush the current error if one is present.
   */
  static void CurrentFlush();

  /**
   * @brief Guard a function call, throwing an error on glfw errors.
   * @note This will only catch the last GLFW error if one is thrown.
   * @{
   */
  template <class F>
    requires (!std::is_void_v<std::invoke_result_t<F>>)
  static auto Guard(F&& func) -> std::invoke_result_t<F> {
    CurrentFlush();
    auto retVal = std::forward<F>(func)();
    auto err = CurrentMaybe();
    if (err.has_value()) {
      throw std::move(*err);
    }

    return retVal;
  }

  template <class F>
    requires (std::is_void_v<std::invoke_result_t<F>>)
  static void Guard(F&& func) {
    CurrentFlush();
    std::forward<F>(func)();
    auto err = CurrentMaybe();
    if (err.has_value()) {
      throw std::move(*err);
    }
  }
  /**
   * @}
   */

private:
  int errCode_;
  const char* description_;
};

/**
 * @brief If this error is thrown, Glfw failed to initialize.
 * @see https://www.glfw.org/docs/3.3/group__errors.html#gad44162d78100ea5e87cdd38426b8c7a1
 */
struct GlfwInitializationError final
    : public WindowInitializationError, public GlfwError {
  GlfwInitializationError();
};

namespace detail {

/**
 * @brief Tiny RAII class for managing the GLFW runtime lifecycle.
 */
struct GlfwRuntime {
  GlfwRuntime() {
    if (glfwInit() != GLFW_TRUE) {
      throw GlfwInitializationError();
    }
  }
  ~GlfwRuntime() {
    glfwTerminate();
  }

  GlfwRuntime(const GlfwRuntime&) = delete;
  auto operator=(const GlfwRuntime&) = delete;
  GlfwRuntime(GlfwRuntime&&) = delete;
  auto operator=(GlfwRuntime&&) = delete;
};

} // namespace detail

class MacOSFrameHandle final : public FrameHandle<MacOSFrameHandle> {
private:
  // The reason this is pimpled is to avoid poluting our C++ codebase with
  // Objective C++ junk I have no intention of dealing with. There is a
  // performance pentalty for this, but I don't care at this moment.
  // TODO(marko): Consider optimizing away this pimpl.
  class Impl;
  std::unique_ptr<Impl> impl_ = nullptr;

public:
  MacOSFrameHandle(const std::string& title,
                   std::pair<std::size_t, std::size_t> dimensions);
  ~MacOSFrameHandle();

  MacOSFrameHandle(const MacOSFrameHandle&) = delete;
  auto operator=(const MacOSFrameHandle&) -> MacOSFrameHandle& = delete;
  MacOSFrameHandle(MacOSFrameHandle&&) = default;
  auto operator=(MacOSFrameHandle&&) -> MacOSFrameHandle& = default;

  void StartRenderingImpl(std::function<void(const RenderInfo&)>&& imRender);
};

class MacOSProvider final
    : public PlatformProvider<MacOSProvider, MacOSFrameHandle> {
public:
  MacOSProvider() : PlatformProvider() {};

  auto CreateFrameImpl(
    const std::string& title,
    std::pair<std::size_t, std::size_t> dimensions
  ) -> MacOSFrameHandle;

private:
  detail::GlfwRuntime glfwRuntime_;
};

} // namespace imsql::os

#else
#error "You have added macos.hpp to your project, but you are not compiling on macOS. Please remove this file or implement the necessary OS provider for your platform."
#endif // __APPLE__
#endif // IMSQLITE_OS_PLATFORM_MACOS_HPP
