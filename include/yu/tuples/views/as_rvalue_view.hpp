// yutool: include guard
#ifndef YU_TUPLES_VIEWS_AS_RVALUE_VIEW_HPP_
#define YU_TUPLES_VIEWS_AS_RVALUE_VIEW_HPP_

#include "adaptor_closure.hpp"
#include "all.hpp"
#include "view_interface.hpp"
#include <yu/tuples/access/index.hpp>
#include <yu/tuples/concepts/view.hpp>
#include <yu/tuples/type_traits/element_type.hpp>
#include <utility>

namespace yu::tuples {

template <view View>
class as_rvalue_view : public view_interface<as_rvalue_view<View>> {
    private:
        View base_;

        template <typename Self>
        constexpr decltype(auto) base(this Self&& self) noexcept {
            return std::forward_like<Self>(self.base_);
        }

    public:
        static constexpr size<View> size;

        constexpr explicit as_rvalue_view(View view) :
            base_(std::move(view)) {}

        template <std::size_t Idx, typename Self>
        requires (Idx < size)
        constexpr decltype(auto) get(this Self&& self) noexcept(
            noexcept(tuples::get(std::forward<Self>(self).base(), index<Idx>))
        ) {
            return std::move(tuples::get(std::forward<Self>(self).base(), index<Idx>));
        }
};

template <typename Tuple>
as_rvalue_view(Tuple&&) -> as_rvalue_view<views::all_t<Tuple&&>>;

namespace views {

inline constexpr adaptor_closure as_rvalue
    = []<tuple Tuple>(Tuple&& tuple) { as_rvalue_view{std::forward<Tuple>(tuple)}; };

} // namespace views

} // namespace yu::tuples

#endif
