// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_LOCATE_LAST_HPP_
#define YU_TUPLES_ALGORITHM_LOCATE_LAST_HPP_

#include "find_last.hpp"
#include <yu/config/stateless_call_operator.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/projected.hpp>
#include <yu/tuples/proxy/location.hpp>
#include <functional>
#include <utility>

namespace yu::tuples {

namespace fn {

struct locate_last_fn {
        template <tuple Tuple, typename T, typename Proj = std::identity>
        constexpr decltype(auto) YU_STATELESS_CALL_OPERATOR(Tuple&& tuple, const T& value, Proj proj = {}) {
            auto&& result = tuples::find_last(std::forward<Tuple>(tuple), value, std::ref(proj));

            return result.transform([&](std::size_t index) { return location{std::forward<Tuple>(tuple), index}; });
        }
};

} // namespace fn

inline namespace _fn {

inline constexpr fn::locate_last_fn locate_last;

}

} // namespace yu::tuples

#endif
