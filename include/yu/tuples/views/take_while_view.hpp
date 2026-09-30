// yutool: include guard
#ifndef YU_TUPLES_VIEWS_TAKE_WHILE_VIEW_HPP_
#define YU_TUPLES_VIEWS_TAKE_WHILE_VIEW_HPP_

#include "_detail/prefix_size.hpp"
#include "_detail/take_view_base.hpp"
#include "all.hpp"
#include "partial_closure.hpp"
#include "view_interface.hpp"
#include <yu/meta/concepts/predicate.hpp>
#include <yu/meta/type.hpp>
#include <yu/tuples/concepts/elementwise_meta_predicate.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/concepts/view.hpp>
#include <yu/tuples/type_traits/element_type.hpp>
#include <utility>

namespace yu::tuples {

template <view View, typename Pred>
requires elementwise_meta_predicate<Pred, View>
class take_while_view :
    public _detail::take_view_base<View, _detail::prefix_size<View, Pred>>,
    public view_interface<take_while_view<View, Pred>> {
    private:
        using base_t = _detail::take_view_base<View, _detail::prefix_size<View, Pred>>;

    public:
        constexpr explicit take_while_view(View view, Pred) :
            base_t(std::move(view)) {}
};

template <typename Tuple, typename Pred>
take_while_view(Tuple&&, Pred) -> take_while_view<views::all_t<Tuple&&>, Pred>;

namespace views {
namespace _unspecified::take_while {

struct adaptor {
        template <tuple Tuple, typename Pred>
        requires elementwise_meta_predicate<Pred, Tuple>
        static constexpr auto operator()(Tuple&& tuple, Pred&& pred) noexcept(
            noexcept(take_while_view{std::forward<Tuple>(tuple), std::forward<Pred>(pred)})
        ) {
            return take_while_view{std::forward<Tuple>(tuple), std::forward<Pred>(pred)};
        }

        template <typename P>
        static constexpr auto operator()(P&& pred) noexcept(
            noexcept(make_partial_closure(adaptor{}, std::forward<P>(pred)))
        ) {
            return make_partial_closure(adaptor{}, std::forward<P>(pred));
        }
};

} // namespace _unspecified::take_while

inline constexpr _unspecified::take_while::adaptor take_while;

} // namespace views

} // namespace yu::tuples

#endif
