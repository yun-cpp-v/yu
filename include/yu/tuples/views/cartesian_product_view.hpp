// yutool: include guard
#ifndef YU_TUPLES_VIEWS_CARTESIAN_PRODUCT_VIEW_HPP_
#define YU_TUPLES_VIEWS_CARTESIAN_PRODUCT_VIEW_HPP_

#include "all.hpp"
#include "single_view.hpp"
#include "view_interface.hpp"
#include <yu/meta/constant.hpp>
#include <yu/tuples/access/get.hpp>
#include <yu/tuples/access/index.hpp>
#include <yu/tuples/access/size.hpp>
#include <yu/tuples/apply.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/concepts/view.hpp>
#include <algorithm>
#include <array>
#include <ranges>
#include <tuple>
#include <type_traits>
#include <utility>

namespace yu::tuples {

template <view... Views>
requires (0 < sizeof...(Views))
class cartesian_product_view : public view_interface<cartesian_product_view<Views...>> {
    private:
        static consteval auto make_indices_table() {
            constexpr auto result = [] consteval {
                auto indices_table_view
                    = std::views::cartesian_product(std::views::iota(0uz, size_v<Views>)...);

                using indices_t            = std::array<std::size_t, sizeof...(Views)>;
                constexpr std::size_t size = (size_v<Views> * ... * 1);

                std::array<indices_t, size> indices_table;

                std::ranges::transform(indices_table_view, indices_table.begin(), [](auto tuple) {
                    return tuples::apply([](auto... index) { return indices_t{index...}; }, tuple);
                });

                return indices_table;
            }();

            return meta::constant<result>;
        }

        static constexpr auto indices_table_ = make_indices_table();

        using indices_table_t = decltype(indices_table_)::value_type;

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
            constexpr auto indices = indices_table_[index<Idx>];

            return []<std::size_t... Dims>(std::index_sequence<Dims...>) {
                return (noexcept(tuples::get(std::declval<Self>().base(index<Dims>), indices[index<Dims>])) && ...);
            }(std::index_sequence_for<Views...>{});
        }

    public:
        static constexpr auto size = meta::constant_invoke(meta::constant<&indices_table_t::size>, indices_table_);

        constexpr explicit cartesian_product_view(
            Views... views
        ) noexcept(std::is_nothrow_constructible_v<bases_t, Views&&...>) :
            bases_(std::move(views)...) {}

        template <std::size_t Idx, typename Self>
        requires (Idx < size)
        constexpr decltype(auto) get(this Self&& self) noexcept(is_nothrow<Idx, Self>()) {
            constexpr auto indices = indices_table_[index<Idx>];

            return [&]<std::size_t... Dims>(std::index_sequence<Dims...>) {
                return std::forward_as_tuple(tuples::get(self.base(index<Dims>), indices[index<Dims>])...);
            }(std::index_sequence_for<Views...>{});
        }
};

template <typename... Tuples>
cartesian_product_view(Tuples&&...) -> cartesian_product_view<views::all_t<Tuples&&>...>;

namespace views {
namespace _unspecified::cartesian_product {

struct adaptor {
        template <tuple... Tuples>
        static constexpr auto operator()(Tuples&&... tuples) noexcept(noexcept(cartesian_product_view{
            std::forward<Tuples>(tuples)...
        })) {
            return cartesian_product_view{std::forward<Tuples>(tuples)...};
        }

        static constexpr auto operator()() noexcept { return views::single(std::tuple{}); }
};

} // namespace _unspecified::cartesian_product

inline constexpr _unspecified::cartesian_product::adaptor cartesian_product{};

} // namespace views

} // namespace yu::tuples

#endif
