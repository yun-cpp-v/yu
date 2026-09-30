// yutool: include guard
#ifndef YU_TUPLES_VIEWS_JOIN_VIEW_HPP_
#define YU_TUPLES_VIEWS_JOIN_VIEW_HPP_

#include "_detail/flatten_view_base.hpp"
#include "_detail/tuple_of_tuples.hpp"
#include "all.hpp"
#include "tuple_adaptor_closure.hpp"
#include "view_interface.hpp"
#include <yu/tuples/access/index.hpp>
#include <yu/tuples/access/size.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/concepts/view.hpp>
#include <yu/tuples/utility/index_sequence_for.hpp>
#include <utility>

namespace yu::tuples {

template <view View>
requires _detail::tuple_of_tuples<View>
class join_view : public _detail::flatten_view_base<View>, public view_interface<join_view<View>> {
    private:
        using base_t = _detail::flatten_view_base<View>;

    public:
        constexpr explicit join_view(View view) :
            base_t(std::move(view)) {}
};

template <typename Tuple>
join_view(Tuple&&) -> join_view<views::all_t<Tuple&&>>;

namespace views {
namespace _unspecified::join {

struct adaptor : public tuple_adaptor_closure<adaptor> {
        template <tuple Tuple>
        requires tuples::_detail::tuple_of_tuples<Tuple>
        static constexpr auto operator()(Tuple&& tuple) noexcept(noexcept(join_view{std::forward<Tuple>(tuple)})) {
            return join_view{std::forward<Tuple>(tuple)};
        }
};

} // namespace _unspecified::join

inline constexpr _unspecified::join::adaptor join{};

} // namespace views

} // namespace yu::tuples

#endif
