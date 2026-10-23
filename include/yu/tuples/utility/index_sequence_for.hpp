// yutool: include guard
#ifndef YU_TUPLES_UTILITY_INDEX_SEQUENCE_FOR_HPP_
#define YU_TUPLES_UTILITY_INDEX_SEQUENCE_FOR_HPP_

#include <yu/tuples/access/size.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <utility>

namespace yu::tuples {

template <tuple T>
using index_sequence_for = std::make_index_sequence<size_v<T>>;

template <tuple T>
inline constexpr index_sequence_for<T> indices_for;

} // namespace yu::tuples

#endif