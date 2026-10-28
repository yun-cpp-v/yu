// yutool: include guard
#ifndef YU_TUPLES_VIEWS_TRANSFORM_VIEW_HPP_
#define YU_TUPLES_VIEWS_TRANSFORM_VIEW_HPP_

#include "adaptor.hpp"
#include "all.hpp"
#include "view_interface.hpp"
#include <yu/tuples/concepts/elementwise_unary_invocable.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/concepts/view.hpp>
#include <yu/tuples/type_traits/element_type.hpp>
#include <concepts>
#include <functional>
#include <type_traits>
#include <utility>

namespace yu::tuples {

template <view View, typename Fn>
requires elementwise_unary_regular_invocable<Fn, View>
class transform_view : public view_interface<transform_view<View, Fn>> {
    private:
        View base_;
        [[no_unique_address]]
        Fn fn_;

        template <typename Self>
        constexpr decltype(auto) base(this Self&& self) noexcept {
            return std::forward_like<Self>(self.base_);
        }

        template <typename Self>
        constexpr decltype(auto) fn(this Self&& self) noexcept {
            return std::forward_like<Self>(self.fn_);
        }

    public:
        static constexpr size<View> size;

        template <typename F>
        requires std::constructible_from<Fn, F&&>
        constexpr explicit transform_view(View view, F&& fn) noexcept(
            std::is_nothrow_move_constructible_v<View> && std::is_nothrow_constructible_v<Fn, F&&>
        ) :
            base_(std::move(view)), fn_(std::forward<F>(fn)) {}

        template <std::size_t Idx, typename Self>
        requires (Idx < size)
        constexpr decltype(auto) get(this Self&& self) noexcept(noexcept(
            std::invoke(std::forward<Self>(self).fn(), tuples::get(std::forward<Self>(self).base(), index<Idx>))
        )) {
            return std::invoke(std::forward<Self>(self).fn(), tuples::get(std::forward<Self>(self).base(), index<Idx>));
        }
};

template <typename Tuple, typename Fn>
transform_view(Tuple&&, Fn) -> transform_view<views::all_t<Tuple&&>, Fn>;

namespace views {

inline constexpr adaptor transform
    = []<tuple Tuple, typename Fn>(Tuple&& tuple, Fn&& fn) requires elementwise_unary_regular_invocable<Fn, Tuple>
{ return transform_view{std::forward<Tuple>(tuple), std::forward<Fn>(fn)}; };

} // namespace views

} // namespace yu::tuples

#endif
