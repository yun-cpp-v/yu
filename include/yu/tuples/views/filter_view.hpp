// yutool: include guard
#ifndef YU_TUPLES_VIEWS_FILTER_VIEW_HPP_
#define YU_TUPLES_VIEWS_FILTER_VIEW_HPP_

#include "_detail/meta_predicate_result_at.hpp"
#include "all.hpp"
#include "partial_closure.hpp"
#include "view_interface.hpp"
#include <yu/meta/concepts/predicate.hpp>
#include <yu/meta/constant.hpp>
#include <yu/meta/type.hpp>
#include <yu/tuples/access/index.hpp>
#include <yu/tuples/concepts/elementwise_meta_predicate.hpp>
#include <yu/tuples/concepts/view.hpp>
#include <yu/tuples/type_traits/element_type.hpp>
#include <algorithm>
#include <array>
#include <cstddef>
#include <ranges>
#include <utility>

namespace yu::tuples {

template <view View, typename Pred>
requires elementwise_meta_predicate<Pred, View>
class filter_view : public view_interface<filter_view<View, Pred>> {
    private:
        static consteval auto make_index_table() {
            constexpr auto result = []<std::size_t... Idx>(std::index_sequence<Idx...>) consteval {
                static constexpr std::array flags = {_detail::meta_predicate_result_at_v<Pred, View, Idx>...};

                auto index_table_view = std::views::iota(0uz, size_v<View>)
                                        | std::views::filter([](std::size_t index) { return flags[index]; });

                constexpr std::size_t size = std::ranges::count(flags, true);

                std::array<std::size_t, size> index_table;

                std::ranges::move(index_table_view, index_table.begin());

                return index_table;
            }(indices_for<View>);

            return meta::constant<result>;
        }

        static constexpr auto index_table_ = make_index_table();
        using index_table_t                = decltype(index_table_)::value_type;

        View base_;

    public:
        static constexpr auto size = meta::constant_invoke(meta::constant<&index_table_t::size>, index_table_);

        constexpr explicit filter_view(View view, Pred) :
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
            noexcept(tuples::get(self.base(), index_table_[index<Idx>]))
        ) {
            return tuples::get(self.base(), index_table_[index<Idx>]);
        }
};

template <typename T, typename P>
filter_view(T&&, P) -> filter_view<views::all_t<T&&>, P>;

namespace views {

namespace _unspecified::filter {

struct adaptor {
        template <tuple Tuple, typename Pred>
        requires elementwise_meta_predicate<Pred, Tuple>
        static constexpr auto operator()(Tuple&& tuple, Pred&& pred) noexcept(
            noexcept(filter_view{std::forward<Tuple>(tuple), std::forward<Pred>(pred)})
        ) {
            return filter_view{std::forward<Tuple>(tuple), std::forward<Pred>(pred)};
        }

        template <typename Pred>
        static constexpr auto operator()(Pred&& pred) noexcept(
            noexcept(make_partial_closure(adaptor{}, std::forward<Pred>(pred)))
        ) {
            return make_partial_closure(adaptor{}, std::forward<Pred>(pred));
        }
};

} // namespace _unspecified::filter

inline constexpr _unspecified::filter::adaptor filter;

} // namespace views

} // namespace yu::tuples

#endif
