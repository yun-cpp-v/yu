// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_LOCATE_IF_NOT_HPP_
#define YU_TUPLES_ALGORITHM_LOCATE_IF_NOT_HPP_

#include "find_if_not.hpp"
#include <yu/config/stateless_call_operator.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/projected.hpp>
#include <yu/tuples/proxy/location.hpp>
#include <functional>
#include <utility>

namespace yu::tuples {

namespace fn {

struct locate_if_not_fn {
        template <tuple Tuple, typename Pred, typename Proj = std::identity>
        constexpr decltype(auto) YU_STATELESS_CALL_OPERATOR(Tuple&& tuple, Pred pred, Proj proj = {}) {
            auto&& result = tuples::find_if_not(std::forward<Tuple>(tuple), std::ref(pred), std::ref(proj));

            return result.transform([&](std::size_t index) { return location{std::forward<Tuple>(tuple), index}; });
        }
};

} // namespace fn

inline namespace _fn {

inline constexpr fn::locate_if_not_fn locate_if_not;

}

} // namespace yu::tuples

#endif
