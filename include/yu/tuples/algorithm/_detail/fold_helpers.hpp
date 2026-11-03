// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_DETAIL_FOLD_HELPERS_HPP_
#define YU_TUPLES_ALGORITHM_DETAIL_FOLD_HELPERS_HPP_

#include <concepts>
#include <type_traits>
#include <utility>

namespace yu::tuples::_detail {

template <typename Stored, typename Fn>
struct lfold_helper {
        Stored stored;
        Fn     fn;

        template <typename Right>
        requires std::invocable<Fn, Stored&&, Right&&>
        constexpr auto operator*(Right&& right) noexcept(std::is_nothrow_invocable_v<Fn, Stored&&, Right&&>) {
            return lfold_helper{fn(std::move(stored), std::forward<Right>(right)), fn};
        }
};

template <typename Arg, typename Fn>
struct rfold_helper {
        Arg arg_;
        Fn  fn_;

        template <typename Left>
        requires std::invocable<Fn, Left&&, Arg&&>
        constexpr auto operator*(Left&& left) noexcept(std::is_nothrow_invocable_v<Fn, Left&&, Arg&&>) {
            return rfold_helper{fn_(std::forward<Left>(left), std::move(arg_)), fn_};
        }
};

} // namespace yu::tuples::_detail

#endif
