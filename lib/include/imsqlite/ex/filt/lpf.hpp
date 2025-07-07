#ifndef IMSQLITE_EX_FILT_LPF_HPP
#define IMSQLITE_EX_FILT_LPF_HPP

#include <numbers>
#include <utility>
#include <chrono>

namespace imsql::ex {

template <class ValueT, class DurationT>
class AlphaLpf {
public:
  explicit AlphaLpf(DurationT cutoffPeriod)
      : cutoffPeriod_(std::move(cutoffPeriod))
  {}

  constexpr auto Observe(const DurationT& samplePeriod, const ValueT& value) {
    const auto alpha_val = alpha(samplePeriod);
    lastValue_ = alpha_val * value + (1.0 - alpha_val) * lastValue_;
    return lastValue_;
  }

  [[nodiscard]] constexpr auto LastValue() const -> const ValueT& {
    return lastValue_;
  }

private:
  [[nodiscard]] constexpr auto alpha(
    const DurationT& samplePeriod
  ) const -> double {
    const auto t_sample =
      std::chrono::duration_cast<std::chrono::duration<double>>(samplePeriod);

    const auto t_cutoff =
      std::chrono::duration_cast<std::chrono::duration<double>>(cutoffPeriod_);

    const auto t_ratio = t_cutoff / t_sample;
    return 1.0 / (1.0 + t_ratio / (2.0 * std::numbers::pi));
  }

  DurationT cutoffPeriod_;
  ValueT lastValue_ = ValueT{};
};

} // namespace imsql::ex

#endif // IMSQLITE_EX_FILT_LPF_HPP
