// yutool: include guard
#ifndef YU_TUPLES_VIEWS_JOIN_WITH_VIEW_HPP_
#define YU_TUPLES_VIEWS_JOIN_WITH_VIEW_HPP_

#include "_detail/flatten_view_base.hpp"
#include "_detail/tuple_of_tuples.hpp"
#include "all.hpp"
#include "partial_closure.hpp"
#include "single_view.hpp"
#include "view_interface.hpp"
#include <yu/tuples/access/get.hpp>
#include <yu/tuples/access/index.hpp>
#include <yu/tuples/access/size.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/concepts/view.hpp>
#include <yu/tuples/utility/index_sequence_for.hpp>
#include <yu/tuples/views/single_view.hpp>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace yu::tuples {

namespace _detail::join_with_view {

template <typename View, typename Pattern>
class separator_insert_view : public tuples::view_interface<separator_insert_view<View, Pattern>> {
    private:
        View    base_;
        Pattern pattern_;

        template <std::size_t Idx, typename Self>
        static consteval bool is_nothrow() {
            if constexpr (Idx % 2 == 0) {
                return noexcept(tuples::get(std::declval<Self>().base(), index<Idx / 2>));
            } else {
                return true;
            }
        }

    public:
        static constexpr index_t<2 * size_v<View> - 1> size{};

        constexpr explicit separator_insert_view(View view, Pattern pattern) noexcept(
            std::is_nothrow_move_constructible_v<View> && std::is_nothrow_move_constructible_v<Pattern>
        ) :
            base_(view), pattern_(pattern) {}

        template <typename Self>
        constexpr decltype(auto) base(this Self&& self) noexcept {
            return std::forward_like<Self>(self.base_);
        }

        template <std::size_t Idx, typename Self>
        requires (Idx < size)
        constexpr decltype(auto) get(this Self&& self) noexcept(is_nothrow<Idx, Self>()) {
            if constexpr (Idx % 2 == 0) {
                return tuples::get(self.base(), index<Idx / 2>);
            } else {
                return std::forward_like<Self>(self.pattern_);
            }
        }
};

} // namespace _detail::join_with_view

template <view View, view Pattern>
requires _detail::tuple_of_tuples<View> 
class join_with_view :
    public _detail::flatten_view_base<_detail::join_with_view::separator_insert_view<View, Pattern>>,
    public view_interface<join_with_view<View, Pattern>> {
    private:
        using inserter_t = _detail::join_with_view::separator_insert_view<View, Pattern>;
        using base_t     = _detail::flatten_view_base<inserter_t>;

    public:
        constexpr explicit join_with_view(View view, Pattern pattern) :
            base_t(inserter_t{std::move(view), std::move(pattern)}) {}
};

template <view View, view Pattern>
requires _detail::tuple_of_tuples<View> && (size_v<View> == 0)
class join_with_view<View, Pattern> : public view_interface<join_with_view<View, Pattern>> {
    public:
        static constexpr index_t<0> size{};

        constexpr explicit join_with_view(View, Pattern) noexcept {}

        template <std::size_t Idx, typename Self>
        constexpr decltype(auto) get(this Self&& self) = delete;
};

template <typename Tuple, typename Pattern>
join_with_view(Tuple&&, Pattern&&) -> join_with_view<views::all_t<Tuple&&>, views::all_t<Pattern&&>>;

namespace views {
namespace _unspecified::join_with {

struct adaptor {
        template <tuple Tuple, tuple Pattern>
        requires tuples::_detail::tuple_of_tuples<Tuple>
        static constexpr auto operator()(Tuple&& tuple, Pattern&& pattern) noexcept(
            noexcept(join_with_view{std::forward<Tuple>(tuple), std::forward<Pattern>(pattern)})
        ) {
            return join_with_view{std::forward<Tuple>(tuple), std::forward<Pattern>(pattern)};
        }

        template <tuple Tuple, typename Pattern>
        requires tuples::_detail::tuple_of_tuples<Tuple>
        static constexpr auto operator()(Tuple&& tuple, Pattern&& pattern) noexcept(
            noexcept(join_with_view{std::forward<Tuple>(tuple), views::single(std::forward<Pattern>(pattern))})
        ) {
            return join_with_view{std::forward<Tuple>(tuple), views::single(std::forward<Pattern>(pattern))};
        }

        template <tuple Pattern>
        static constexpr auto operator()(Pattern&& pattern) noexcept(
            noexcept(make_partial_closure(adaptor{}, views::all(std::forward<Pattern>(pattern))))
        ) {
            return make_partial_closure(adaptor{}, std::forward<Pattern>(pattern));
        }

        template <typename Pattern>
        static constexpr auto operator()(Pattern&& pattern) noexcept(
            noexcept(make_partial_closure(adaptor{}, std::forward<Pattern>(pattern)))
        ) {
            return make_partial_closure(adaptor{}, std::forward<Pattern>(pattern));
        }
};

} // namespace _unspecified::join_with

inline constexpr _unspecified::join_with::adaptor join_with{};

} // namespace views
} // namespace yu::tuples

#endif
