// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_LOCATE_HPP_
#define YU_TUPLES_ALGORITHM_LOCATE_HPP_

#include "find.hpp"
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/projected.hpp>
#include <yu/tuples/proxy/location.hpp>
#include <functional>
#include <utility>

namespace yu::tuples {

namespace _unspecified::locate {

struct fn {
        template <tuple Tuple, typename T, typename Proj = std::identity>
                static constexpr decltype(auto) operator()(Tuple&& tuple, const T& value, Proj proj = {}) {
            auto&& result = tuples::find(std::forward<Tuple>(tuple), value, std::ref(proj));

            return result.transform([&](std::size_t index) { return location{std::forward<Tuple>(tuple), index}; });
        }
};

} // namespace _unspecified::locate

inline namespace _fn {

inline constexpr _unspecified::locate::fn locate{};

}

} // namespace yu::tuples

#endif
