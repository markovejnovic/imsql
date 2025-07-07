#ifndef IMSQLITE_UI_WINDBG_HPP
#define IMSQLITE_UI_WINDBG_HPP
#include "imgui.h"
#pragma once

#include "imsqlite/os/platform/frame_handle.hpp"
#include "imsqlite/deprecated.hpp"

namespace imsql::ui {
class WinDbg final : public Window {
public:
  explicit WinDbg(const char* fpsStr) : Window("Debug Window") {
    ImGui::TextUnformatted(fpsStr);
  }
};

} // namespace imsql::ui

#endif // IMSQLITE_UI_WINDBG_HPP
