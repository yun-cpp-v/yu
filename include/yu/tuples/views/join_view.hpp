// yutool: include guard
#ifndef YU_TUPLES_VIEWS_JOIN_VIEW_HPP_
#define YU_TUPLES_VIEWS_JOIN_VIEW_HPP_

#include "_detail/tuple_of_tuples.hpp"
#include "all.hpp"
#include "tuple_adaptor_closure.hpp"
#include "view_interface.hpp"
#include <yu/meta/constant.hpp>
#include <yu/tuples/access/index.hpp>
#include <yu/tuples/access/size.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/concepts/view.hpp>
#include <yu/tuples/type_traits/element_type.hpp>
#include <yu/tuples/utility/index_sequence_for.hpp>
#include <algorithm>
#include <array>
#include <cstddef>
#include <utility>

namespace yu::tuples {

template <view View>
requires _detail::tuple_of_tuples<View>
class join_view : public view_interface<join_view<View>> {
    private:
        struct index_pair {
                std::size_t base_index;
                std::size_t inner_index;
        };

        static consteval auto make_index_map() {
            constexpr auto result = []<std::size_t... Idx>(std::index_sequence<Idx...>) consteval {
                constexpr std::array  base_indices = {size_v<element_type_t<Idx, View>>...};
                constexpr std::size_t size         = (size_v<element_type_t<Idx, View>> + ... + 0);

                std::array<index_pair, size> index_map;

                std::size_t flat = 0;
                for (std::size_t base = 0; base < sizeof...(Idx); ++base) {
                    for (std::size_t inner = 0; inner < base_indices[base]; ++inner) {
                        index_map[flat++] = {base, inner};
                    }
                }

                return index_map;
            }(indices_for<View>);

            return meta::constant<result>;
        }

        static constexpr auto index_map_ = make_index_map();
        using index_map_t                = decltype(index_map_)::value_type;

        View base_;

        template <typename Self>
        [[nodiscard]]
        constexpr decltype(auto) base(this Self&& self) noexcept {
            return std::forward_like<Self>(self.base_);
        }

        template <std::size_t Idx, typename Self>
        static consteval bool is_nothrow() {
            constexpr auto map         = index_map_[index<Idx>];
            constexpr auto base_index  = meta::constant_invoke(meta::constant<&index_pair::base_index>, map);
            constexpr auto inner_index = meta::constant_invoke(meta::constant<&index_pair::inner_index>, map);

            return noexcept(tuples::get(tuples::get(std::declval<Self>().base(), base_index), inner_index));
        }

    public:
        static constexpr auto size = meta::constant_invoke(meta::constant<&index_map_t::size>, index_map_);

        constexpr explicit join_view(View base) :
            base_(std::move(base)) {}

        template <std::size_t Idx, typename Self>
        requires (Idx < size)
        [[nodiscard]]
        constexpr decltype(auto) get(this Self&& self) noexcept(is_nothrow<Idx, Self>()) {
            constexpr auto map         = index_map_[index<Idx>];
            constexpr auto base_index  = meta::constant_invoke(meta::constant<&index_pair::base_index>, map);
            constexpr auto inner_index = meta::constant_invoke(meta::constant<&index_pair::inner_index>, map);

            return tuples::get(tuples::get(self.base(), base_index), inner_index);
        }
};

template <typename Tuple>
join_view(Tuple&&) -> join_view<views::all_t<Tuple&&>>;

namespace views {
namespace _unspecified::join {

struct adaptor : public tuple_adaptor_closure<adaptor> {
        template <tuple Tuple>
        requires tuples::_detail::tuple_of_tuples<Tuple>
        static constexpr auto operator()(Tuple&& tuple) noexcept(noexcept(join_view{std::forward<Tuple>(tuple)})) {
            return join_view{std::forward<Tuple>(tuple)};
        }
};

} // namespace _unspecified::join

inline constexpr _unspecified::join::adaptor join{};

} // namespace views

} // namespace yu::tuples

#endif
