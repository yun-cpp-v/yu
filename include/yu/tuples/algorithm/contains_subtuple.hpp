// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_CONTAINS_SUBTUPLE_HPP_
#define YU_TUPLES_ALGORITHM_CONTAINS_SUBTUPLE_HPP_

#include "search.hpp"
#include <yu/config/stateless_call_operator.hpp>
#include <yu/functional/predicates.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/projected.hpp>
#include <functional>
#include <utility>

namespace yu::tuples {

namespace fn {

struct contains_subtuple_fn {
        template <
            tuple Tuple1,
            tuple Tuple2,
            typename Pred  = functional::safe_equal_to,
            typename Proj1 = std::identity,
            typename Proj2 = std::identity
        >
        constexpr bool YU_STATELESS_CALL_OPERATOR(
            Tuple1&& tuple1,
            Tuple2&& tuple2,
            Pred     pred  = {},
            Proj1    proj1 = {},
            Proj2    proj2 = {}
        ) {
            auto&& result = tuples::search(
                std::forward<Tuple1>(tuple1),
                std::forward<Tuple2>(tuple2),
                std::ref(pred),
                std::ref(proj1),
                std::ref(proj2)
            );

            return result.has_value();
        }
};

} // namespace fn

inline namespace _fn {

inline constexpr fn::contains_subtuple_fn contains_subtuple;

}

} // namespace yu::tuples

#endif
