// yutool: include guard
#ifndef YU_TUPLES_VIEWS_ENUMERATE_VIEW_HPP_
#define YU_TUPLES_VIEWS_ENUMERATE_VIEW_HPP_

#include "adaptor_closure.hpp"
#include "all.hpp"
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
        static constexpr size<View> size;

        constexpr explicit enumerate_view(View view) :
            base_(std::move(view)) {}

        template <std::size_t Idx, typename Self>
        requires (Idx < size)
        constexpr decltype(auto) get(this Self&& self) noexcept(
            noexcept(tuples::get(std::forward<Self>(self).base(), index<Idx>))
        ) {
            using result_t
                = std::tuple<index_t<Idx>, decltype(tuples::get(std::forward<Self>(self).base(), index<Idx>))&&>;

            return result_t{index<Idx>, tuples::get(std::forward<Self>(self).base(), index<Idx>)};
        }
};

template <typename Tuple>
enumerate_view(Tuple&&) -> enumerate_view<views::all_t<Tuple&&>>;

namespace views {

inline constexpr adaptor_closure enumerate
    = []<tuple Tuple>(Tuple&& tuple) { return enumerate_view{std::forward<Tuple>(tuple)}; };

} // namespace views

} // namespace yu::tuples

#endif
