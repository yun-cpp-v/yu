// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_NONE_OF_HPP_
#define YU_TUPLES_ALGORITHM_NONE_OF_HPP_

#include "any_of.hpp"
#include <yu/config/stateless_call_operator.hpp>
#include <yu/tuples/concepts/elementwise_unary_predicate.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/projected.hpp>
#include <functional>
#include <utility>

namespace yu::tuples {

namespace fn {

struct none_of_fn {
        template <tuple Tuple, typename Pred, typename Proj = std::identity>
        requires elementwise_unary_predicate<Pred, projected<Tuple, Proj>>
        constexpr bool YU_STATELESS_CALL_OPERATOR(Tuple&& tuple, Pred pred, Proj proj = {}) {
            return !tuples::any_of(std::forward<Tuple>(tuple), std::ref(pred), std::ref(proj));
        }
};

} // namespace fn

inline namespace _fn {

inline constexpr fn::none_of_fn none_of;

}

} // namespace yu::tuples

#endif
