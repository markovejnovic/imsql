#ifndef IMSQLITE_UI_WINDBG_HPP
#define IMSQLITE_UI_WINDBG_HPP
#pragma once

#include "imsqlite/os/platform/frame_handle.hpp"
#include "window.hpp"

namespace imsql::ui {
class WinDbg final : public Window {
public:
  explicit WinDbg(const os::RenderInfo& renderInfo);
};

} // namespace imsql::ui

#endif // IMSQLITE_UI_WINDBG_HPP
