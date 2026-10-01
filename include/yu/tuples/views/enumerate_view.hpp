// yutool: include guard
#ifndef YU_TUPLES_VIEWS_ENUMERATE_VIEW_HPP_
#define YU_TUPLES_VIEWS_ENUMERATE_VIEW_HPP_

#include "all.hpp"
#include "tuple_adaptor_closure.hpp"
#include "view_interface.hpp"
#include <yu/tuples/access/index.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/concepts/view.hpp>
#include <yu/tuples/type_traits/element_type.hpp>
#include <tuple>
#include <utility>

namespace yu::tuples {

template <view View>
class enumerate_view : public view_interface<enumerate_view<View>> {
    private:
        View base_;

        template <typename Self>
        constexpr decltype(auto) base(this Self&& self) noexcept {
            return std::forward_like<Self>(self.base_);
        }

    public:
        static constexpr size<View> size{};

        constexpr explicit enumerate_view(View view) :
            base_(std::move(view)) {}

        template <std::size_t Idx, typename Self>
        requires (Idx < size)
        [[nodiscard]]
        constexpr decltype(auto) get(this Self&& self) noexcept(noexcept(tuples::get(self.base(), index<Idx>))) {
            using result_t = std::tuple<index_t<Idx>, element_type_t<Idx, decltype(self.base())>&&>;

            return result_t{index<Idx>, tuples::get(self.base(), index<Idx>)};
        }
};

template <typename Tuple>
enumerate_view(Tuple&&) -> enumerate_view<views::all_t<Tuple&&>>;

namespace views {

namespace _unspecified::enumerate {

struct closure : tuple_adaptor_closure<closure> {
        template <tuple Tuple>
        static constexpr auto operator()(Tuple&& tuple) noexcept(noexcept(enumerate_view{std::forward<Tuple>(tuple)})) {
            return enumerate_view{std::forward<Tuple>(tuple)};
        }
};

} // namespace _unspecified::enumerate

inline constexpr _unspecified::enumerate::closure enumerate{};

} // namespace views

} // namespace yu::tuples

#endif
