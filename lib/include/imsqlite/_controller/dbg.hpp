#ifndef IMSQLITE_CONTROLLER_DBG_HPP
#define IMSQLITE_CONTROLLER_DBG_HPP

#include <chrono>
#include "imsqlite/_view/dbg.hpp"
#include "imsqlite/ex/filt/lpf.hpp"

namespace imsql::controller {

struct Dbg {
private:
  using TimeType = std::chrono::duration<double>;

public:
  Dbg() noexcept = default;

  constexpr void ObserveFrameTime(
    std::chrono::microseconds frameTime
  ) noexcept {
    fpsLpf_.Observe(frameTime, frameTime);
  }

  [[nodiscard]] constexpr auto View() const noexcept -> view::Dbg {
    const auto time_as_us =
      std::chrono::duration_cast<std::chrono::microseconds>(
        fpsLpf_.LastValue());

    return view::Dbg{time_as_us};
  }

private:
  static constexpr auto kFpsCutoffPeriod = std::chrono::milliseconds(200);

  ex::AlphaLpf<TimeType, TimeType> fpsLpf_{
    std::chrono::duration_cast<TimeType>(kFpsCutoffPeriod)
  };
};

} // namespace imsql::controller

#endif // IMSQLITE_CONTROLLER_DBG_HPP
