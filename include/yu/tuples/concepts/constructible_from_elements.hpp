// yutool: include guard
#ifndef YU_TUPLES_CONCEPTS_CONSTRUCTIBLE_FROM_ELEMENTS_HPP_
#define YU_TUPLES_CONCEPTS_CONSTRUCTIBLE_FROM_ELEMENTS_HPP_

#include <yu/tuples/type_traits/element_type.hpp>
#include <yu/tuples/utility/index_sequence_for.hpp>
#include <concepts>
#include <cstddef>
#include <utility>

namespace yu::tuples {

template <typename T, typename Tuple, typename... Args>
concept constructible_from_elements = []<std::size_t... Idx>(std::index_sequence<Idx...>) {
    return std::constructible_from<T, element_type_t<Idx, Tuple>..., Args...>;
}(indices_for<Tuple>);

} // namespace yu::tuples

#endif
