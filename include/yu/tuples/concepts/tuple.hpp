// yutool: include guard
#ifndef YU_TUPLES_CONCEPTS_TUPLE_HPP_
#define YU_TUPLES_CONCEPTS_TUPLE_HPP_

#include <yu/tuples/access/get.hpp>
#include <yu/tuples/access/index.hpp>
#include <yu/tuples/access/size.hpp>
#include <utility>

namespace yu::tuples {

namespace _detail::tuple {

template <typename T, std::size_t... Idx>
consteval bool impl(std::index_sequence<Idx...>) {
    return (requires(T&& t) { tuples::get(std::forward<T>(t), index<Idx>); } && ...);
}

} // namespace _detail::tuple

template <typename T>
concept tuple = _detail::tuple::impl<T>(std::make_index_sequence<size<std::remove_reference_t<T>>::value>{});

} // namespace yu::tuples

#endif
