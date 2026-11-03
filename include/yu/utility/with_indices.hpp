// yutool: include guard
#ifndef YU_UTILITY_WITH_INDICES_HPP_
#define YU_UTILITY_WITH_INDICES_HPP_

#include "constant.hpp"
#include <cstddef>
#include <type_traits>
#include <utility>

namespace yu {

namespace _detail::with_indices {

template <typename Fn, std::size_t... Idx>
requires std::invocable<Fn&&, constant_t<Idx>...>
constexpr decltype(auto) impl(std::index_sequence<Idx...>, Fn&& fn) noexcept(
    std::is_nothrow_invocable_v<Fn&&, constant_t<Idx>...>
) {
    return std::forward<Fn>(fn)(yu::constant<Idx>...);
}

} // namespace _detail::with_indices

template <std::size_t Idx, typename Fn>
constexpr auto with_indices(Fn&& fn) noexcept(
    noexcept(_detail::with_indices::impl(std::make_index_sequence<Idx>{}, std::forward<Fn>(fn)))
) {
    return _detail::with_indices::impl(std::make_index_sequence<Idx>{}, std::forward<Fn>(fn));
}

} // namespace yu

#endif
