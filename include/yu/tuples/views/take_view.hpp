// yutool: include guard
#ifndef YU_TUPLES_VIEWS_TAKE_VIEW_HPP_
#define YU_TUPLES_VIEWS_TAKE_VIEW_HPP_

#include "all.hpp"
#include "slice_view.hpp"
#include "tuple_adaptor_closure.hpp"
#include "view_interface.hpp"
#include <yu/config/constructor_object.hpp>
#include <yu/tuples/access/index.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/concepts/view.hpp>
#include <yu/tuples/type_traits/element_type.hpp>
#include <cstddef>
#include <utility>

namespace yu::tuples {

template <view View, std::size_t Count>
class take_view : public view_interface<take_view<View, Count>> {
    private:
        slice_view<View, 0, Count> base_;

        template <typename Self>
        constexpr decltype(auto) base(this Self&& self) noexcept {
            return std::forward_like<Self>(self.base_);
        }

    public:
        static constexpr auto size = slice_view<View, 0, Count>::size;

        template <std::size_t Idx, typename Self>
        requires (Idx < size)
        constexpr decltype(auto) get(this Self&& self) noexcept(
            noexcept(tuples::get(std::forward<Self>(self).base(), index<Idx>))
        ) {
            return tuples::get(std::forward<Self>(self).base(), index<Idx>);
        }
};

template <typename Tuple, std::size_t Count>
take_view(Tuple&&, index_t<Count>) -> take_view<views::all_t<Tuple&&>, Count>;

namespace views {

inline constexpr adaptor take = YU_CONSTRUCTOR_OBJECT(take_view);

} // namespace views

} // namespace yu::tuples

#endif
