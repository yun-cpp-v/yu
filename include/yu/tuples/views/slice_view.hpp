// yutool: include guard
#ifndef YU_TUPLES_VIEWS_SLICE_VIEW_HPP_
#define YU_TUPLES_VIEWS_SLICE_VIEW_HPP_

#include "all.hpp"
#include "tuple_adaptor_closure.hpp"
#include "view_interface.hpp"
#include <yu/config/constructor_object.hpp>
#include <yu/tuples/access/index.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/concepts/view.hpp>
#include <yu/tuples/type_traits/element_type.hpp>
#include <algorithm>
#include <cstddef>
#include <utility>

namespace yu::tuples {

template <view View, std::size_t Begin, std::size_t End>
requires (Begin <= End)
class slice_view : public view_interface<slice_view<View, Begin, End>> {
    private:
        View base_;

        template <typename Self>
        constexpr decltype(auto) base(this Self&& self) noexcept {
            return std::forward_like<Self>(self.base_);
        }

    public:
        static constexpr auto size = index<std::min(size_v<View>, End - Begin)>;

        constexpr explicit slice_view(View view, index_t<Begin>, index_t<End>) :
            base_(std::move(view)) {}

        template <std::size_t Idx, typename Self>
        requires (Idx < size)
        constexpr decltype(auto) get(this Self&& self) noexcept(
            noexcept(tuples::get(std::forward<Self>(self).base(), index<Begin + Idx>))
        ) {
            return tuples::get(std::forward<Self>(self).base(), index<Begin + Idx>);
        }
};

template <typename Tuple, std::size_t Begin, std::size_t End>
slice_view(Tuple&&, index_t<Begin>, index_t<End>) -> slice_view<views::all_t<Tuple&&>, Begin, End>;

namespace views {

inline constexpr adaptor slice = YU_CONSTRUCTOR_OBJECT(slice_view);

} // namespace views

} // namespace yu::tuples

#endif
