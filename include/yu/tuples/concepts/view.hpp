// yutool: include guard
#ifndef YU_TUPLES_CONCEPTS_VIEW_HPP_
#define YU_TUPLES_CONCEPTS_VIEW_HPP_

#include "tuple.hpp"
#include <yu/tuples/views/enable_view.hpp>
#include <concepts>

namespace yu::tuples {

template <typename T>
concept view = tuple<T> && std::movable<T> && enable_view<T>;

} // namespace yu::tuples

#endif
