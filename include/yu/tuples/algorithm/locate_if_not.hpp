// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_LOCATE_IF_NOT_HPP_
#define YU_TUPLES_ALGORITHM_LOCATE_IF_NOT_HPP_

#include "find_if_not.hpp"
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/projected.hpp>
#include <yu/tuples/proxy/location.hpp>
#include <functional>
#include <utility>

namespace yu::tuples {

namespace _unspecified::locate_if_not {

struct fn {
    public:
        template <tuple Tuple, typename Pred, typename Proj = std::identity>
        static constexpr decltype(auto) operator()(Tuple&& tuple, Pred pred, Proj proj = {}) {
            auto&& result = tuples::find_if_not(std::forward<Tuple>(tuple), std::ref(pred), std::ref(proj));

            return result.transform([&](std::size_t index) { return location{std::forward<Tuple>(tuple), index}; });
        }
};

} // namespace _unspecified::locate_if_not

inline namespace _fn {

inline constexpr _unspecified::locate_if_not::fn locate_if_not;

}

} // namespace yu::tuples

#endif
