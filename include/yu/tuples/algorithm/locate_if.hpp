// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_LOCATE_IF_HPP_
#define YU_TUPLES_ALGORITHM_LOCATE_IF_HPP_

#include "find_if.hpp"
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/projected.hpp>
#include <yu/tuples/proxy/location.hpp>
#include <functional>
#include <utility>

namespace yu::tuples {

namespace _unspecified::locate_if {

struct fn {
    public:
        template <tuple Tuple, typename Pred, typename Proj = std::identity>
                static constexpr auto operator()(Tuple&& tuple, Pred pred, Proj proj = {}) {
            auto&& result = tuples::find_if(std::forward<Tuple>(tuple), std::ref(pred), std::ref(proj));

            return result.transform([&](std::size_t index) { return location{std::forward<Tuple>(tuple), index}; });
        }
};

} // namespace _unspecified::locate_if

inline namespace _fn {

inline constexpr _unspecified::locate_if::fn locate_if{};

}

} // namespace yu::tuples

#endif
