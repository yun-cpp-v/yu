// yutool: include guard
#ifndef YU_TUPLES_VIEWS_ZIP_VIEW_HPP_
#define YU_TUPLES_VIEWS_ZIP_VIEW_HPP_

#include "all.hpp"
#include "empty_view.hpp"
#include "view_interface.hpp"
#include "yu/tuples/access/get.hpp"
#include <yu/meta/constant.hpp>
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
            noexcept(tuples::get(self.bases(), idx))
        ) {
            return tuples::get(self.bases(), idx);
        }

        template <std::size_t Idx, typename Self>
        static consteval bool is_nothrow() {
            return [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                return (noexcept(tuples::get(std::declval<Self>().base(index<Is>), index<Idx>)) && ...);
            }(std::index_sequence_for<Views...>{});
        }

    public:
        static constexpr index_t<std::min({size_v<Views>...})> size{};

        constexpr explicit zip_view(Views... views) :
            bases_(std::move(views)...) {}

        template <std::size_t Idx, typename Self>
        requires (Idx < size)
        constexpr decltype(auto) get(this Self&& self) noexcept(is_nothrow<Idx, Self>()) {
            return [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                return std::forward_as_tuple(tuples::get(self.base(index<Is>), index<Idx>)...);
            }(std::index_sequence_for<Views...>{});
        }
};

template <typename... Views>
zip_view(Views&&...) -> zip_view<views::all_t<Views&&>...>;

namespace views {
namespace _unspecified::zip {

struct adaptor {
        template <tuple... Tuples>
        requires (0 < sizeof...(Tuples))
        static constexpr auto operator()(Tuples&&... tuples) noexcept(noexcept(zip_view{
            std::forward<Tuples>(tuples)...
        })) {
            return zip_view{std::forward<Tuples>(tuples)...};
        }

        static constexpr auto operator()() noexcept { return empty; }
};

} // namespace _unspecified::zip

inline constexpr _unspecified::zip::adaptor zip{};

} // namespace views

} // namespace yu::tuples

#endif
