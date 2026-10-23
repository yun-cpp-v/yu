// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_MATCH_HPP_
#define YU_TUPLES_ALGORITHM_MATCH_HPP_

#include "search.hpp"
#include <yu/functional/predicates.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/projected.hpp>
#include <yu/tuples/proxy/segment.hpp>
#include <cstddef>
#include <functional>
#include <utility>

namespace yu::tuples {

namespace _unspecified::match {

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
            auto&& result = tuples::search(
                std::forward<Tuple1>(tuple1),
                std::forward<Tuple2>(tuple2),
                std::ref(pred),
                std::ref(proj1),
                std::ref(proj2)
            );

            return result.transform([&](std::size_t index) {
                return segment{std::forward<Tuple1>(tuple1), index, size_v<Tuple2>};
            });
        }
};

} // namespace _unspecified::match

inline namespace _fn {

inline constexpr _unspecified::match::fn match;

}

} // namespace yu::tuples

#endif
