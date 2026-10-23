// yutool: include guard
#ifndef YU_TUPLES_ACCESS_INDEX_HPP_
#define YU_TUPLES_ACCESS_INDEX_HPP_

#include <yu/meta/constant.hpp>
#include <cstddef>

namespace yu::tuples {

template <std::size_t V>
using index_t = meta::constant_t<V>;

template <std::size_t Idx>
inline constexpr index_t<Idx> index;

} // namespace yu::tuples

#endif
