// yutool: include guard
#ifndef YU_TUPLES_VIEWS_CONCAT_VIEW_HPP_
#define YU_TUPLES_VIEWS_CONCAT_VIEW_HPP_

#include "all.hpp"
#include "join_view.hpp"
#include "view_interface.hpp"
#include <yu/tuples/access/index.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/concepts/view.hpp>
#include <yu/tuples/utility/index_sequence_for.hpp>
#include <tuple>
#include <type_traits>
#include <utility>

namespace yu::tuples {

template <view... Views>
class concat_view : public view_interface<concat_view<Views...>> {
    private:
        using base_tuple_t = std::tuple<Views...>;
        using base_t       = join_view<base_tuple_t>;

        base_t base_;

        template <typename Self>
        [[nodiscard]]
        constexpr decltype(auto) base(this Self&& self) noexcept {
            return std::forward_like<Self>(self.base_);
        }

    public:
        static constexpr auto size = base_t::size;

        constexpr explicit concat_view(Views... views) :
            base_(base_tuple_t{std::move(views)...}) {}

        template <std::size_t Idx, typename Self>
        requires (Idx < size)
        [[nodiscard]]
        constexpr decltype(auto) get(this Self&& self) noexcept(noexcept(tuples::get(self.base(), index<Idx>))) {
            return tuples::get(self.base(), index<Idx>);
        }
};

template <typename... Tuples>
concat_view(Tuples&&...) -> concat_view<views::all_t<Tuples&&>...>;

namespace views {
namespace _unspecified::concat {

struct adaptor {
        template <tuple... Tuples>
        static constexpr auto operator()(Tuples&&... tuples) noexcept(noexcept(concat_view{
            std::forward<Tuples>(tuples)...
        })) {
            return concat_view{std::forward<Tuples>(tuples)...};
        }
};

} // namespace _unspecified::concat

inline constexpr _unspecified::concat::adaptor concat{};

} // namespace views

} // namespace yu::tuples

#endif
