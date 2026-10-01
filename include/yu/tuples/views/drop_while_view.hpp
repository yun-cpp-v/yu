// yutool: include guard
#ifndef YU_TUPLES_VIEWS_DROP_WHILE_VIEW_HPP_
#define YU_TUPLES_VIEWS_DROP_WHILE_VIEW_HPP_

#include "_detail/prefix_size.hpp"
#include "all.hpp"
#include "drop_view.hpp"
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
class drop_while_view : public view_interface<drop_while_view<View, Pred>> {
    private:
        using base_t = drop_view<View, _detail::prefix_size<View, Pred>>;

        base_t base_;

        template <typename Self>
        [[nodiscard]]
        constexpr decltype(auto) base(this Self&& self) noexcept {
            return std::forward_like<Self>(self.base_);
        }

    public:
        static constexpr auto size = base_t::size;

        constexpr explicit drop_while_view(View view, Pred) :
            base_(std::move(view)) {}

        template <std::size_t Idx, typename Self>
        requires (Idx < size)
        [[nodiscard]]
        constexpr decltype(auto) get(this Self&& self) noexcept(noexcept(tuples::get(self.base(), index<Idx>))) {
            return tuples::get(self.base(), index<Idx>);
        }
};

template <typename Tuple, typename Pred>
drop_while_view(Tuple&&, Pred) -> drop_while_view<views::all_t<Tuple&&>, Pred>;

namespace views {
namespace _unspecified::drop_while {

struct adaptor {
        template <tuple Tuple, typename Pred>
        requires elementwise_meta_predicate<Pred, Tuple>
        static constexpr auto operator()(Tuple&& tuple, Pred&& pred) noexcept(
            noexcept(drop_while_view{std::forward<Tuple>(tuple), std::forward<Pred>(pred)})
        ) {
            return drop_while_view{std::forward<Tuple>(tuple), std::forward<Pred>(pred)};
        }

        template <typename Pred>
        static constexpr auto operator()(Pred&& pred) noexcept(
            noexcept(make_partial_closure(adaptor{}, std::forward<Pred>(pred)))
        ) {
            return make_partial_closure(adaptor{}, std::forward<Pred>(pred));
        }
};

} // namespace _unspecified::drop_while

inline constexpr _unspecified::drop_while::adaptor drop_while;

} // namespace views

} // namespace yu::tuples

#endif
