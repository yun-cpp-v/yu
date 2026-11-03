// yutool: include guard
#ifndef YU_TUPLES_VIEWS_JOIN_VIEW_HPP_
#define YU_TUPLES_VIEWS_JOIN_VIEW_HPP_

#include "all.hpp"
#include "tuple_adaptor_closure.hpp"
#include "view_interface.hpp"
#include <yu/config/constructor_object.hpp>
#include <yu/tuples/access/index.hpp>
#include <yu/tuples/access/size.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/concepts/tuple_of_tuples.hpp>
#include <yu/tuples/concepts/view.hpp>
#include <yu/tuples/type_traits/element_type.hpp>
#include <yu/utility/constant.hpp>
#include <yu/utility/with_indices.hpp>
#include <algorithm>
#include <array>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace yu::tuples {

template <view View>
requires tuple_of_tuples<View>
class join_view : public view_interface<join_view<View>> {
    private:
        struct index_pair {
                std::size_t base_index;
                std::size_t inner_index;
        };

        static consteval auto make_index_map() {
            constexpr auto result = with_indices<size_v<std::remove_reference_t<View>>>([](auto... idx) {
                constexpr std::array  base_indices = {size_v<element_type_t<std::size_t(idx), View>>...};
                constexpr std::size_t size         = (size_v<element_type_t<std::size_t(idx), View>> + ... + 0);

                std::array<index_pair, size> index_map;

                std::size_t flat = 0;
                for (std::size_t base = 0; base < sizeof...(idx); ++base) {
                    for (std::size_t inner = 0; inner < base_indices[base]; ++inner) {
                        index_map[flat++] = {base, inner};
                    }
                }

                return index_map;
            });

            return constant<result>;
        }

        static constexpr auto index_map_ = make_index_map();
        using index_map_t                = decltype(index_map_)::value_type;

        View base_;

        template <typename Self>
        constexpr decltype(auto) base(this Self&& self) noexcept {
            return std::forward_like<Self>(self.base_);
        }

        template <std::size_t Idx, typename Self>
        static consteval bool is_nothrow() {
            constexpr auto map         = index_map_[index<Idx>];
            constexpr auto base_index  = constant_invoke(constant<&index_pair::base_index>, map);
            constexpr auto inner_index = constant_invoke(constant<&index_pair::inner_index>, map);

            return noexcept(tuples::get(tuples::get(std::declval<Self>().base(), base_index), inner_index));
        }

    public:
        static constexpr auto size = constant_invoke(constant<&index_map_t::size>, index_map_);

        constexpr explicit join_view(View base) :
            base_(std::move(base)) {}

        template <std::size_t Idx, typename Self>
        requires (Idx < size)
        constexpr decltype(auto) get(this Self&& self) noexcept(is_nothrow<Idx, Self>()) {
            constexpr auto map         = index_map_[index<Idx>];
            constexpr auto base_index  = constant_invoke(constant<&index_pair::base_index>, map);
            constexpr auto inner_index = constant_invoke(constant<&index_pair::inner_index>, map);

            return tuples::get(tuples::get(std::forward<Self>(self).base(), base_index), inner_index);
        }
};

template <typename Tuple>
join_view(Tuple&&) -> join_view<views::all_t<Tuple&&>>;

namespace views {

inline constexpr adaptor join = YU_CONSTRUCTOR_OBJECT(join_view);

} // namespace views

} // namespace yu::tuples

#endif
