// yutool: include guard
#ifndef YU_TUPLES_VIEWS_CONCAT_VIEW_HPP_
#define YU_TUPLES_VIEWS_CONCAT_VIEW_HPP_

#include "all.hpp"
#include "join_view.hpp"
#include "tuple_adaptor_closure.hpp"
#include "view_interface.hpp"
#include <yu/config/constructor_object.hpp>
#include <yu/tuples/access/index.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/concepts/view.hpp>
#include <tuple>
#include <utility>

namespace yu::tuples {

template <view... Views>
class concat_view : public view_interface<concat_view<Views...>> {
    private:
        join_view<std::tuple<Views...>> base_;

        template <typename Self>
        constexpr decltype(auto) base(this Self&& self) noexcept {
            return std::forward_like<Self>(self.base_);
        }

    public:
        static constexpr auto size = join_view<std::tuple<Views...>>::size;

        constexpr explicit concat_view(Views... views) :
            base_(std::forward_as_tuple(std::move(views)...)) {}

        template <std::size_t Idx, typename Self>
        requires (Idx < size)
        constexpr decltype(auto) get(this Self&& self) noexcept(
            noexcept(tuples::get(std::forward<Self>(self).base(), index<Idx>))
        ) {
            return tuples::get(std::forward<Self>(self).base(), index<Idx>);
        }
};

template <typename... Tuples>
concat_view(Tuples&&...) -> concat_view<views::all_t<Tuples&&>...>;

namespace views {

inline constexpr adaptor concat = YU_CONSTRUCTOR_OBJECT(concat_view);

} // namespace views

} // namespace yu::tuples

#endif
