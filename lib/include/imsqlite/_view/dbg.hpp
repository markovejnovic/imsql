#ifndef IMSQLITE_VIEW_DBG_HPP
#define IMSQLITE_VIEW_DBG_HPP

#include "imsqlite/ui/windbg.hpp"
#include <chrono>

namespace imsql::view {

struct Dbg {
  explicit constexpr Dbg(std::chrono::microseconds frameTime)
      : frameTime_(frameTime) {}

  [[nodiscard]] constexpr auto Fps() const noexcept -> float {
    using namespace std::chrono_literals;

    const auto frame_period =
      std::chrono::duration_cast<std::chrono::duration<float>>(frameTime_);

    return 1.0F / frame_period.count();
  }

  auto Render() const noexcept -> ui::WinDbg {
    return ui::WinDbg { std::format("FPS: {:.0f}", Fps()).c_str() };
  }

private:
  std::chrono::microseconds frameTime_;
};

} // namespace imsql::view

#endif // IMSQLITE_VIEW_DBG_HPP
