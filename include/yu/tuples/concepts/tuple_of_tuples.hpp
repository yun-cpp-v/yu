// yutool: include guard
#ifndef YU_TUPLES_CONCEPTS_TUPLE_OF_TUPLES_HPP_
#define YU_TUPLES_CONCEPTS_TUPLE_OF_TUPLES_HPP_

#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/type_traits/element_type.hpp>
#include <yu/utility/with_indices.hpp>
#include <cstddef>
#include <type_traits>

namespace yu::tuples {

template <typename Tuple>
concept tuple_of_tuples = with_indices<size<std::remove_reference_t<Tuple>>::value>([](auto... idx) {
    return (tuples::tuple<element_type_t<std::size_t(idx), Tuple>> && ...);
});

} // namespace yu::tuples

#endif
