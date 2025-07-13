/// @note AI generated
#ifndef IMSQLITE_UTIL_VIEWS_DEDUP_HPP
#define IMSQLITE_UTIL_VIEWS_DEDUP_HPP

#include <concepts>
#include <ranges>
#include <utility>

namespace imsql::vw {

template <std::ranges::forward_range V>
requires std::ranges::view<V> && std::equality_comparable<std::ranges::range_value_t<V>>
class Dedup : public std::ranges::view_interface<Dedup<V>> {
private:
  V base_;

  class iterator {
    std::ranges::iterator_t<V> current_;
    std::ranges::sentinel_t<V> end_;
    bool done_ = false;

  public:
    using value_type = std::ranges::range_value_t<V>;
    using difference_type = std::ranges::range_difference_t<V>;
    using iterator_category = std::input_iterator_tag;
    using iterator_concept = std::input_iterator_tag;

    iterator() = default;
    iterator(std::ranges::iterator_t<V> curr, std::ranges::sentinel_t<V> end)
        : current_(std::move(curr)), end_(std::move(end)) {
      if (current_ == end_) done_ = true;
    }

    value_type operator*() const {
      return *current_;
    }

    iterator& operator++() {
      const auto& val = *current_;
      do {
        ++current_;
      } while (current_ != end_ && *current_ == val);
      if (current_ == end_) done_ = true;
      return *this;
    }

    void operator++(int) { ++(*this); }

    friend bool operator==(const iterator& it, std::default_sentinel_t) {
      return it.done_;
    }

    friend bool operator!=(const iterator& it, std::default_sentinel_t) {
      return !it.done_;
    }
  };


public:
  Dedup() = default;
  explicit Dedup(V base) : base_(std::move(base)) {}

  auto begin() {
    return iterator{std::ranges::begin(base_), std::ranges::end(base_)};
  }

  auto end() {
    return std::default_sentinel;
  }
};

// Pipeable adaptor
inline constexpr struct DedupFn {
  template <std::ranges::viewable_range R>
  auto operator()(R&& rng) const {
    return Dedup{std::views::all(std::forward<R>(rng))};
  }

  friend auto operator|(std::ranges::viewable_range auto&& rng, DedupFn fn) {
    return fn(std::forward<decltype(rng)>(rng));
  }
} dedup;

} // namespace imsql::vw

#endif // IMSQLITE_UTIL_VIEWS_DEDUP_HPP
