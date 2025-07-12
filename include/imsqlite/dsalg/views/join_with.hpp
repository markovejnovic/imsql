#ifndef IMSQLITE_DSALG_VIEWS_JOIN_WITH_HPP
#define IMSQLITE_DSALG_VIEWS_JOIN_WITH_HPP

#include <ranges>
#include <utility>

namespace imsql::vw {

template <std::ranges::input_range V, typename T>
requires std::ranges::view<V> && std::copyable<T>
class intersperse_view : public std::ranges::view_interface<intersperse_view<V, T>> {
    V base_;
    T separator_;

    class iterator {
        using BaseIter = std::ranges::iterator_t<V>;
        BaseIter current_;
        BaseIter end_;
        T* separator_;
        bool on_separator_ = false;

    public:
        using value_type = std::ranges::range_value_t<V>;
        using reference = std::common_reference_t<std::ranges::range_reference_t<V>, T&>;
        using difference_type = std::ranges::range_difference_t<V>;
        using iterator_category = std::input_iterator_tag;

        iterator() = default;
        iterator(BaseIter begin, BaseIter end, T& sep)
            : current_(begin), end_(end), separator_(&sep) {}

        reference operator*() const {
            if (on_separator_)
                return *separator_;
            return *current_;
        }

        iterator& operator++() {
            if (on_separator_) {
                ++current_;
                on_separator_ = false;
            } else {
                if (std::next(current_) != end_) {
                    on_separator_ = true;
                } else {
                    ++current_;
                }
            }
            return *this;
        }

        void operator++(int) { ++*this; }

        bool operator==(std::default_sentinel_t) const {
            return current_ == end_;
        }
    };

public:
    intersperse_view() = default;
    intersperse_view(V base, T separator)
        : base_(std::move(base)), separator_(std::move(separator)) {}

    auto begin() { return iterator(std::ranges::begin(base_), std::ranges::end(base_), separator_); }
    auto end() { return std::default_sentinel; }
};

template <class R, class T>
intersperse_view(R&&, T) -> intersperse_view<std::views::all_t<R>, T>;

template <typename T>
struct intersperse_closure {
    T separator;

    template <std::ranges::viewable_range R>
    friend auto operator|(R&& r, const intersperse_closure& self) {
        return intersperse_view(std::views::all(std::forward<R>(r)), self.separator);
    }
};

struct intersperse_fn {
    template <typename T>
    auto operator()(T sep) const {
        return intersperse_closure<T>{std::move(sep)};
    }

    template <std::ranges::viewable_range R, typename T>
    auto operator()(R&& r, T sep) const {
        return intersperse_view(std::views::all(std::forward<R>(r)), std::move(sep));
    }
};

inline constexpr intersperse_fn intersperse;

} // namespace imsql::vw

#endif // IMSQLITE_DSALG_VIEWS_JOIN_WITH_HPP
