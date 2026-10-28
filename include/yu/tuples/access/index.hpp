// yutool: include guard
#ifndef YU_TUPLES_ACCESS_INDEX_HPP_
#define YU_TUPLES_ACCESS_INDEX_HPP_

#include <yu/meta/constant.hpp>
#include <cstddef>

namespace yu::tuples {

/**
    `std::size_t`型の定数をNTTPに包んで値として扱うための型．
*/
template <std::size_t N>
using index_t = meta::constant_t<N>;

/**
    `index_t<N>`の値を簡潔に生成するための変数テンプレート．
*/
template <std::size_t N>
inline constexpr index_t<N> index;

} // namespace yu::tuples

#endif
