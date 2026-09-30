// yutool: include guard
#ifndef YU_TUPLES_VIEWS_REVERSE_VIEW_HPP_
#define YU_TUPLES_VIEWS_REVERSE_VIEW_HPP_

#include "all.hpp"
#include "tuple_adaptor_closure.hpp"
#include "view_interface.hpp"
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/concepts/view.hpp>
#include <yu/tuples/type_traits/element_type.hpp>
#include <utility>

namespace yu::tuples {

template <view View>
class reverse_view : public view_interface<reverse_view<View>> {
    private:
        View base_;

    public:
        static constexpr size<View> size{};

        constexpr explicit reverse_view(View view) :
            base_(std::move(view)) {}

        template <typename Self>
        [[nodiscard]]
        constexpr decltype(auto) base(this Self&& self) noexcept {
            return std::forward_like<Self>(self.base_);
        }

        template <std::size_t Idx, typename Self>
        requires (Idx < size)
        [[nodiscard]]
        constexpr decltype(auto) get(this Self&& self) noexcept(
            noexcept(tuples::get(self.base(), index<size - Idx - 1>))
        ) {
            return tuples::get(self.base(), index<size - Idx - 1>);
        }
};

template <typename Tuple>
reverse_view(Tuple&&) -> reverse_view<views::all_t<Tuple&&>>;

namespace views {

namespace _unspecified::reverse {

struct closure : tuple_adaptor_closure<closure> {
        template <tuple Tuple>
        static constexpr auto operator()(Tuple&& tuple) noexcept(noexcept(reverse_view{std::forward<Tuple>(tuple)})) {
            return reverse_view{std::forward<Tuple>(tuple)};
        }
};

} // namespace _unspecified::reverse

inline constexpr _unspecified::reverse::closure reverse{};

} // namespace views

} // namespace yu::tuples

#endif
