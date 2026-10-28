// yutool: include guard
#ifndef YU_TUPLES_VIEWS_ZIP_VIEW_HPP_
#define YU_TUPLES_VIEWS_ZIP_VIEW_HPP_

#include "adaptor.hpp"
#include "all.hpp"
#include "empty_view.hpp"
#include "view_interface.hpp"
#include <yu/meta/constant.hpp>
#include <yu/tuples/access/get.hpp>
#include <yu/tuples/access/index.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/concepts/view.hpp>
#include <algorithm>
#include <tuple>
#include <utility>

namespace yu::tuples {

template <view... Views>
requires (0 < sizeof...(Views))
class zip_view : public view_interface<zip_view<Views...>> {
    private:
        std::tuple<Views...> bases_;
        using bases_t = std::tuple<Views...>;

        template <typename Self>
        constexpr decltype(auto) bases(this Self&& self) noexcept {
            return std::forward_like<Self>(self.bases_);
        }

        template <std::size_t Idx, typename Self>
        constexpr decltype(auto) base(this Self&& self, index_t<Idx> idx) noexcept(
            noexcept(tuples::get(std::forward<Self>(self).bases(), idx))
        ) {
            return tuples::get(std::forward<Self>(self).bases(), idx);
        }

        template <std::size_t Idx, typename Self>
        static consteval bool is_nothrow() {
            return [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                return (noexcept(tuples::get(std::declval<Self>().base(index<Is>), index<Idx>)) && ...);
            }(std::index_sequence_for<Views...>{});
        }

    public:
        static constexpr index_t<std::min({size_v<Views>...})> size;

        constexpr explicit zip_view(Views... views) :
            bases_(std::move(views)...) {}

        template <std::size_t Idx, typename Self>
        requires (Idx < size)
        constexpr decltype(auto) get(this Self&& self) noexcept(is_nothrow<Idx, Self>()) {
            return [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                return std::forward_as_tuple(tuples::get(std::forward<Self>(self).base(index<Is>), index<Idx>)...);
            }(std::index_sequence_for<Views...>{});
        }
};

template <typename... Views>
zip_view(Views&&...) -> zip_view<views::all_t<Views&&>...>;

namespace views {

inline constexpr adaptor zip = []<tuple... Tuples>(Tuples&&... tuples) {
    if constexpr (0 < sizeof...(Tuples)) {
        return zip_view{std::forward<Tuples>(tuples)...};
    } else {
        return empty;
    }
};

} // namespace views

} // namespace yu::tuples

#endif
