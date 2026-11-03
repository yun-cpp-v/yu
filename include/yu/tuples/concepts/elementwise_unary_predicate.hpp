// yutool: include guard
#ifndef YU_TUPLES_CONCEPTS_ELEMENTWISE_UNARY_PREDICATE_HPP_
#define YU_TUPLES_CONCEPTS_ELEMENTWISE_UNARY_PREDICATE_HPP_

#include <yu/tuples/type_traits/element_type.hpp>
#include <yu/utility/with_indices.hpp>
#include <concepts>
#include <cstddef>
#include <type_traits>

namespace yu::tuples {

template <typename Fn, typename Tuple>
concept elementwise_unary_predicate = with_indices<size<std::remove_reference_t<Tuple>>::value>([](auto... idx) {
    return (std::predicate<Fn, element_type_t<std::size_t(idx), Tuple>> && ...);
});

} // namespace yu::tuples

#endif
