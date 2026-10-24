// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_CONTAINS_SUBTUPLE_HPP_
#define YU_TUPLES_ALGORITHM_CONTAINS_SUBTUPLE_HPP_

#include "search.hpp"
#include <yu/functional/predicates.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/projected.hpp>
#include <functional>
#include <utility>

namespace yu::tuples {

namespace _unspecified::contains_subtuple {

struct fn {
    public:
        template <
            tuple Tuple1,
            tuple Tuple2,
            typename Pred  = functional::safe_equal_to,
            typename Proj1 = std::identity,
            typename Proj2 = std::identity
        >
        static constexpr bool operator()(
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

} // namespace _unspecified::contains_subtuple

inline namespace _fn {

inline constexpr _unspecified::contains_subtuple::fn contains_subtuple;

}

} // namespace yu::tuples

#endif
