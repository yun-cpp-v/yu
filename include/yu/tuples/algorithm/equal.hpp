// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_EQUAL_HPP_
#define YU_TUPLES_ALGORITHM_EQUAL_HPP_

#include <yu/config/stateless_call_operator.hpp>
#include <yu/functional/predicates.hpp>
#include <yu/functional/with_fallback.hpp>
#include <yu/tuples/access/get.hpp>
#include <yu/tuples/access/size.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/projected.hpp>
#include <yu/tuples/utility/index_sequence_for.hpp>
#include <cstddef>
#include <functional>
#include <utility>

namespace yu::tuples {

namespace fn {

struct equal_fn {
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
            if constexpr (size_v<Tuple1> != size_v<Tuple2>) {
                return false;
            } else {
                auto safe_pred = functional::with_fallback(std::ref(pred), false);

                return [&]<std::size_t... Idx>(std::index_sequence<Idx...>) {
                    return (
                        std::invoke(
                            safe_pred,
                            std::invoke(proj1, tuples::get(std::forward<Tuple1>(tuple1), index<Idx>)),
                            std::invoke(proj2, tuples::get(std::forward<Tuple2>(tuple2), index<Idx>))
                        )
                        && ...
                    );
                }(indices_for<Tuple1>);
            }
        }
};

} // namespace fn

inline namespace _fn {

inline constexpr fn::equal_fn equal;

}

} // namespace yu::tuples

#endif
