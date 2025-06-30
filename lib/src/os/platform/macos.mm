#include "imsqlite/os/platform/macos.hpp"
#include <chrono>
#include <gsl/narrow>
#include <optional>
#include <stdexcept>
#include <utility>
#include "GLFW/glfw3.h"
#include "imsqlite/os/platform/frame_handle.hpp"
#include <Metal/Metal.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#include "GLFW/glfw3native.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_metal.h"
#include <Cocoa/Cocoa.h>
#include <QuartzCore/QuartzCore.h>
#include <format>

namespace imsql::os {

GlfwError::GlfwError(int errCode, const char* description)
  : PlatformError(std::format("GLFW Error {}: {}", errCode, description))
  , errCode_(errCode)
  , description_(description) {}

auto GlfwError::CurrentMaybe() -> std::optional<GlfwError> {
  const char* description = "";
  const int errorCode = glfwGetError(&description);

  if (errorCode == GLFW_NO_ERROR) {
    return std::nullopt;
  }

  return GlfwError{errorCode, description};
}

auto GlfwError::Current() -> GlfwError {
  auto maybeError = CurrentMaybe();
  if (maybeError.has_value()) {
    return *maybeError;
  }

  throw std::runtime_error(
    "Failed to retrieve GLFW error: no error reported"
  );
}

auto GlfwError::CurrentFlush() -> void {
  (void)glfwGetError(nullptr);
}

GlfwInitializationError::GlfwInitializationError()
  : WindowInitializationError("Critical error initializing GLFW.")
  , GlfwError(GLFW_PLATFORM_ERROR, "Unspecified GLFW_PLATFORM_ERROR.") {}

class MacOSFrameHandle::Impl {
public: 
  Impl(const std::string& title, std::pair<std::size_t, std::size_t> dimensions)
      : window_(([&]() -> GLFWwindow* {
        GlfwError::Guard([]() {
          glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        });

        return GlfwError::Guard([&]() -> GLFWwindow* {
          return glfwCreateWindow(gsl::narrow<int>(dimensions.first),
                                  gsl::narrow<int>(dimensions.second),
                                  title.c_str(), nullptr, nullptr);
        });
      }()))
      , device_(MTLCreateSystemDefaultDevice())
      , winCmdQueue_([device_ newCommandQueue])
        {
        if (!ImGui_ImplGlfw_InitForOpenGL(window_, true)) {
          throw WindowInitializationError(
            "Failed to initialize ImGui for OpenGL.");
        }

        if (!ImGui_ImplMetal_Init(device_)) {
          throw WindowInitializationError(
            "Failed to initialize ImGui for Metal.");
        }

        NSWindow* ns_win = GlfwError::Guard([&]() {
          return glfwGetCocoaWindow(window_);
        });

        metalLayer_ = [CAMetalLayer layer];
        metalLayer_.device = device_;
        metalLayer_.pixelFormat = MTLPixelFormatBGRA8Unorm;
        ns_win.contentView.layer = metalLayer_;
        ns_win.contentView.wantsLayer = YES;

        renderPassDescriptor_ = [MTLRenderPassDescriptor renderPassDescriptor];
      }

  ~Impl() {
    ImGui_ImplMetal_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    if (window_ != nullptr) {
      glfwDestroyWindow(window_);
    }
  }

  Impl(const Impl&) = delete;
  auto operator=(const Impl&) = delete;
  Impl(Impl&&) = delete;
  auto operator=(Impl&&) = delete;

  void StartRenderingImpl(std::function<void(const RenderInfo&)>&& imRender) {
    std::chrono::steady_clock::duration frame_duration;

    while (glfwWindowShouldClose(window_) == 0) {
      const auto start_ts = std::chrono::steady_clock::now();
      @autoreleasepool {
        glfwPollEvents();

        std::pair<int, int> win_dims;
        glfwGetFramebufferSize(window_, &win_dims.first, &win_dims.second);
        metalLayer_.drawableSize = CGSizeMake(win_dims.first, win_dims.second);

        id<CAMetalDrawable> drawable = [metalLayer_ nextDrawable];
        id<MTLCommandBuffer> cmd_buf = [winCmdQueue_ commandBuffer];
        renderPassDescriptor_.colorAttachments[0].clearColor
          = MTLClearColorMake(0.0, 0.0, 0.0, 1.0);
        renderPassDescriptor_.colorAttachments[0].texture = drawable.texture;
        renderPassDescriptor_.colorAttachments[0].loadAction =
          MTLLoadActionClear;
        renderPassDescriptor_.colorAttachments[0].storeAction =
          MTLStoreActionStore;

        id <MTLRenderCommandEncoder> render_encoder
          = [cmd_buf renderCommandEncoderWithDescriptor:renderPassDescriptor_];
        [render_encoder pushDebugGroup:@"ImSqlite Render"];

        ImGui_ImplMetal_NewFrame(renderPassDescriptor_);
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        imRender(RenderInfo{
          std::chrono::duration_cast<std::chrono::microseconds>(frame_duration),
        });

        ImGui::Render();
        ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), cmd_buf,
                                       render_encoder);

        [render_encoder popDebugGroup];
        [render_encoder endEncoding];
        [cmd_buf presentDrawable:drawable];
        [cmd_buf commit];
      }
      const auto end_ts = std::chrono::steady_clock::now();
      frame_duration = end_ts - start_ts;
    }
  }

private:
  GLFWwindow* window_;
  id<MTLDevice> device_;
  id<MTLCommandQueue> winCmdQueue_;
  CAMetalLayer* metalLayer_ = nil;
  MTLRenderPassDescriptor* renderPassDescriptor_ = nil;
};

MacOSFrameHandle::MacOSFrameHandle(
  const std::string& title,
  std::pair<std::size_t, std::size_t> dimensions
) : impl_(std::make_unique<Impl>(title, std::move(dimensions))) {}

MacOSFrameHandle::~MacOSFrameHandle() = default;

void MacOSFrameHandle::StartRenderingImpl(
  std::function<void(const RenderInfo&)>&& imRender
) {
  impl_->StartRenderingImpl(std::move(imRender));
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
auto MacOSProvider::CreateFrameImpl(
  const std::string& title,
  std::pair<std::size_t, std::size_t> dimensions
) -> MacOSFrameHandle {
  return{title, std::move(dimensions)};
}

} // namespace imsql::os
