// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_MATCH_N_HPP_
#define YU_TUPLES_ALGORITHM_MATCH_N_HPP_

#include "search_n.hpp"
#include <yu/functional/predicates.hpp>
#include <yu/tuples/access/index.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/projected.hpp>
#include <yu/tuples/proxy/segment.hpp>
#include <cstddef>
#include <functional>
#include <utility>

namespace yu::tuples {

namespace _unspecified::match_n {

struct fn {
    public:
        template <
            tuple Tuple,
            typename T,
            std::size_t Count,
            typename Pred = functional::safe_equal_to,
            typename Proj = std::identity
        >
                static constexpr auto operator()(
            Tuple&&        tuple,
            const T&       value,
            index_t<Count> count,
            Pred           pred = {},
            Proj           proj = {}
        ) {
            auto&& result = tuples::search_n(std::forward<Tuple>(tuple), value, count, std::ref(pred), std::ref(proj));

            return result.transform([&](std::size_t index) {
                return segment{std::forward<Tuple>(tuple), index, Count};
            });
        }
};

} // namespace _unspecified::match_n

inline namespace _fn {

inline constexpr _unspecified::match_n::fn match_n;

}

} // namespace yu::tuples

#endif
