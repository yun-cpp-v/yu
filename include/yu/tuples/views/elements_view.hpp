// yutool: include guard
#ifndef YU_TUPLES_VIEWS_ELEMENTS_VIEW_HPP_
#define YU_TUPLES_VIEWS_ELEMENTS_VIEW_HPP_

#include "all.hpp"
#include "tuple_adaptor_closure.hpp"
#include "view_interface.hpp"
#include <yu/config/constructor_object.hpp>
#include <yu/tuples/access/index.hpp>
#include <yu/tuples/access/size.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/concepts/tuple_of_tuples.hpp>
#include <yu/tuples/concepts/view.hpp>
#include <yu/utility/with_indices.hpp>
#include <type_traits>
#include <utility>

namespace yu::tuples {

namespace _detail::elements_view {

template <std::size_t N, typename T>
concept each_tuple_gettable_at = with_indices<size_v<std::remove_reference_t<T>>>([](auto... idx) {
    return ((N < size_v<element_type_t<std::size_t(idx), T>>) && ...);
});

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

inline constexpr adaptor elements = YU_CONSTRUCTOR_OBJECT(elements_view);

} // namespace views

} // namespace yu::tuples

#endif
