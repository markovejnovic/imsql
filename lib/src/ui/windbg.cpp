#include "imsqlite/ui/windbg.hpp"
#include "imgui.h"
#include <chrono>
#include <format>

namespace imsql::ui {

WinDbg::WinDbg(const os::RenderInfo& renderInfo)
    : Window("Debug Window") {
  using namespace std::chrono_literals;

  const std::chrono::duration<double, std::micro> frameTime =
      renderInfo.FrameTime();

  const std::string fpsStr = std::format("{:.2f}", 1.0 / (frameTime / 1s));

  ImGui::PlotLines("FPS", NULL, 0, 0, fpsStr.c_str());
}

} // namespace imsql::ui
