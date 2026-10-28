// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_COUNT_HPP_
#define YU_TUPLES_ALGORITHM_COUNT_HPP_

#include "count_if.hpp"
#include <yu/config/stateless_call_operator.hpp>
#include <yu/functional/predicates.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/projected.hpp>
#include <cstddef>
#include <functional>
#include <utility>

namespace yu::tuples {

namespace fn {

struct count_fn {
        template <tuple Tuple, typename T, typename Proj = std::identity>
        constexpr std::size_t YU_STATELESS_CALL_OPERATOR(Tuple&& tuple, const T& value, Proj proj = {}) {
            return tuples::count_if(
                std::forward<Tuple>(tuple),
                std::bind_back(functional::safe_equal_to{}, value),
                std::ref(proj)
            );
        }
};

} // namespace fn

inline namespace _fn {

inline constexpr fn::count_fn count;

}

} // namespace yu::tuples

#endif
