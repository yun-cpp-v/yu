// yutool: include guard
#ifndef YU_TUPLES_CONCEPTS_APPLICABLE_HPP_
#define YU_TUPLES_CONCEPTS_APPLICABLE_HPP_

#include <yu/tuples/apply.hpp>

namespace yu::tuples {

template <typename Fn, typename Tuple>
concept applicable = std::invocable<decltype(apply), Fn&&, Tuple&&>;

template <typename Fn, typename Tuple>
concept regular_applicable = applicable<Fn, Tuple>;

} // namespace yu::tuples

#endif
