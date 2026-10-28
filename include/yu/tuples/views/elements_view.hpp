// yutool: include guard
#ifndef YU_TUPLES_VIEWS_ELEMENTS_VIEW_HPP_
#define YU_TUPLES_VIEWS_ELEMENTS_VIEW_HPP_

#include "adaptor.hpp"
#include "all.hpp"
#include "view_interface.hpp"
#include <yu/tuples/access/index.hpp>
#include <yu/tuples/access/size.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/concepts/tuple_of_tuples.hpp>
#include <yu/tuples/concepts/view.hpp>
#include <yu/tuples/utility/index_sequence_for.hpp>
#include <utility>

namespace yu::tuples {

namespace _detail::elements_view {

template <std::size_t N, typename T, std::size_t... Idx>
consteval bool each_tuple_gettable_at_impl(std::index_sequence<Idx...>) {
    return ((N < size_v<element_type_t<Idx, T>>) && ...);
}

template <std::size_t N, typename T>
concept each_tuple_gettable_at = each_tuple_gettable_at_impl<N, T>(indices_for<T>);

} // namespace _detail::elements_view

template <view View, std::size_t N>
requires tuple_of_tuples<View> && _detail::elements_view::each_tuple_gettable_at<N, View>
class elements_view : public view_interface<elements_view<View, N>> {
    private:
        View base_;

        template <typename Self>
        constexpr decltype(auto) base(this Self&& self) noexcept {
            return std::forward_like<Self>(self.base_);
        }

    public:
        static constexpr size<View> size{};

        constexpr explicit elements_view(View view, index_t<N>) :
            base_(std::move(view)) {}

        template <std::size_t Idx, typename Self>
        requires (Idx < size)
        constexpr decltype(auto) get(this Self&& self) noexcept(
            noexcept(tuples::get(tuples::get(std::forward<Self>(self).base(), index<Idx>), index<N>))
        ) {
            return tuples::get(tuples::get(std::forward<Self>(self).base(), index<Idx>), index<N>);
        }
};

template <typename Tuple, std::size_t N>
elements_view(Tuple&&, index_t<N>) -> elements_view<views::all_t<Tuple&&>, N>;

namespace views {

inline constexpr adaptor elements = []<tuple_of_tuples Tuple, std::size_t N>(Tuple&& tuple, index_t<N> n)
                                        requires tuples::_detail::elements_view::each_tuple_gettable_at<N, Tuple>
{ return elements_view{std::forward<Tuple>(tuple), n}; };

} // namespace views

} // namespace yu::tuples

#endif
