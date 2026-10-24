// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_LOCATE_FIRST_OF_HPP_
#define YU_TUPLES_ALGORITHM_LOCATE_FIRST_OF_HPP_

#include "find_first_of.hpp"
#include <yu/functional/predicates.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/projected.hpp>
#include <yu/tuples/proxy/location.hpp>
#include <cstddef>
#include <functional>
#include <utility>

namespace yu::tuples {

namespace _unspecified::locate_first_of {

struct fn {
    public:
        template <
            tuple Tuple1,
            tuple Tuple2,
            typename Pred  = functional::safe_equal_to,
            typename Proj1 = std::identity,
            typename Proj2 = std::identity
        >
        static constexpr auto operator()(
            Tuple1&& tuple1,
            Tuple2&& tuple2,
            Pred     pred  = {},
            Proj1    proj1 = {},
            Proj2    proj2 = {}
        ) {
            auto&& result = tuples::find_first_of(
                std::forward<Tuple1>(tuple1),
                std::forward<Tuple2>(tuple2),
                std::ref(pred),
                std::ref(proj1),
                std::ref(proj2)
            );

            return result.transform([&](std::size_t index) { return location{std::forward<Tuple1>(tuple1), index}; });
        }
};

} // namespace _unspecified::locate_first_of

inline namespace _fn {

inline constexpr _unspecified::locate_first_of::fn locate_first_of;

}

} // namespace yu::tuples

#endif
