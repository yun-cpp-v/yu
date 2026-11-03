// yutool: include guard
#ifndef YU_TUPLES_CONCEPTS_CONSTRUCTIBLE_FROM_ELEMENTS_HPP_
#define YU_TUPLES_CONCEPTS_CONSTRUCTIBLE_FROM_ELEMENTS_HPP_

#include <yu/tuples/type_traits/element_type.hpp>
#include <yu/utility/with_indices.hpp>
#include <concepts>
#include <cstddef>
#include <type_traits>

namespace yu::tuples {

template <typename T, typename Tuple, typename... Args>
concept constructible_from_elements = with_indices<size<std::remove_reference_t<Tuple>>::value>([](auto... idx) {
    return std::constructible_from<T, element_type_t<std::size_t(idx), Tuple>..., Args...>;
});

} // namespace yu::tuples

#endif
