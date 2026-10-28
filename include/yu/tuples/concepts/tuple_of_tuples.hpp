// yutool: include guard
#ifndef YU_TUPLES_CONCEPTS_TUPLE_OF_TUPLES_HPP_
#define YU_TUPLES_CONCEPTS_TUPLE_OF_TUPLES_HPP_

#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/type_traits/element_type.hpp>
#include <yu/tuples/utility/index_sequence_for.hpp>
#include <cstddef>
#include <utility>

namespace yu::tuples {

namespace _detail::tuple_of_tuples {

template <typename T, std::size_t... Idx>
consteval bool impl(std::index_sequence<Idx...>) {
    return (tuples::tuple<element_type_t<Idx, T>> && ...);
}

} // namespace _detail::tuple_of_tuples

template <typename Tuple>
concept tuple_of_tuples = tuples::tuple<Tuple> && _detail::tuple_of_tuples::impl<Tuple>(indices_for<Tuple>);

} // namespace yu::tuples

#endif
