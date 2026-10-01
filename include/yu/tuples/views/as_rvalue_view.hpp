// yutool: include guard
#ifndef YU_TUPLES_VIEWS_AS_RVALUE_VIEW_HPP_
#define YU_TUPLES_VIEWS_AS_RVALUE_VIEW_HPP_

#include "all.hpp"
#include "tuple_adaptor_closure.hpp"
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
        static constexpr size<View> size{};

        constexpr explicit as_rvalue_view(View view) :
            base_(std::move(view)) {}

        template <std::size_t Idx, typename Self>
        requires (Idx < size)
        [[nodiscard]]
        constexpr decltype(auto) get(this Self&& self) noexcept(noexcept(tuples::get(self.base(), index<Idx>))) {
            return std::move(tuples::get(self.base(), index<Idx>));
        }
};

template <typename Tuple>
as_rvalue_view(Tuple&&) -> as_rvalue_view<views::all_t<Tuple&&>>;

namespace views {
namespace _unspecified::as_rvalue {

struct closure : tuple_adaptor_closure<closure> {
        template <tuple Tuple>
        static constexpr auto operator()(Tuple&& tuple) noexcept(noexcept(as_rvalue_view{std::forward<Tuple>(tuple)})) {
            return as_rvalue_view{std::forward<Tuple>(tuple)};
        }
};

} // namespace _unspecified::as_rvalue

inline constexpr _unspecified::as_rvalue::closure as_rvalue{};

} // namespace views

} // namespace yu::tuples

#endif
