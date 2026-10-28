// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_FIND_LAST_IF_NOT_HPP_
#define YU_TUPLES_ALGORITHM_FIND_LAST_IF_NOT_HPP_

#include "find_last_if.hpp"
#include <yu/config/stateless_call_operator.hpp>
#include <yu/tuples/concepts/elementwise_unary_predicate.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/projected.hpp>
#include <yu/tuples/utility/index_sequence_for.hpp>
#include <cstddef>
#include <functional>
#include <utility>

namespace yu::tuples {

namespace fn {

struct find_last_if_not_fn {
        template <tuple Tuple, typename Pred, typename Proj = std::identity>
        constexpr std::optional<std::size_t> YU_STATELESS_CALL_OPERATOR(Tuple&& tuple, Pred pred, Proj proj = {}) {
            return tuples::find_last_if(
                std::forward<Tuple>(tuple),
                std::not_fn(functional::with_fallback(std::ref(pred), false)),
                std::ref(proj)
            );
        }
};

} // namespace fn

inline namespace _fn {

inline constexpr fn::find_last_if_not_fn find_last_if_not;

}

} // namespace yu::tuples

#endif
