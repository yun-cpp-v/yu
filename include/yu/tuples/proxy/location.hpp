// yutool: include guard
#ifndef YU_TUPLES_PROXY_LOCATION_HPP_
#define YU_TUPLES_PROXY_LOCATION_HPP_

#include "yu/tuples/access/size.hpp"
#include <yu/tuples/concepts/view.hpp>
#include <yu/tuples/utility/index_sequence_for.hpp>
#include <yu/tuples/views/all.hpp>
#include <array>
#include <cstddef>
#include <functional>
#include <type_traits>
#include <utility>
#include <variant>

namespace yu::tuples {

namespace _detail::location {

template <std::size_t... Idx>
constexpr auto make_index_variant_table(std::index_sequence<Idx...>) {
    using dispatch_variant = std::variant<index_t<Idx>...>;

    return std::array{dispatch_variant{index<Idx>}...};
}

} // namespace _detail::location

template <view View>
class location {
    private:
        static constexpr auto index_variant_table_ = _detail::location::make_index_variant_table(indices_for<View>);

        View        view_;
        std::size_t index_;

    public:
        constexpr explicit location(View view, std::size_t index) :
            view_(std::move(view)), index_(index) {}

        template <typename Self>
        [[nodiscard]]
        constexpr decltype(auto) base(this Self&& self) noexcept {
            return std::forward_like<Self>(self.view_);
        }

        [[nodiscard]]
        constexpr std::size_t index() const noexcept {
            return index_;
        }

        template <typename Self, typename Visitor>
        [[nodiscard]]
        constexpr decltype(auto) visit(this Self&& self, Visitor&& visitor) {
            return std::visit(
                [&]<std::size_t Idx>(index_t<Idx> idx) -> decltype(auto) {
                    return std::invoke(std::forward<Visitor>(visitor), tuples::get(self.base(), idx));
                },
                index_variant_table_[self.index_]
            );
        }
};

template <view View>
requires (0 == size_v<View>)
class location<View> {
    public:
        constexpr explicit location(View, std::size_t) noexcept {}
};

template <typename Tuple>
location(Tuple&&, std::size_t) -> location<views::all_t<Tuple&&>>;

} // namespace yu::tuples

#endif
