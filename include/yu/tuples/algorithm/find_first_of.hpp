// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_FIND_FIRST_OF_HPP_
#define YU_TUPLES_ALGORITHM_FIND_FIRST_OF_HPP_

#include "any_of.hpp"
#include <yu/functional/predicates.hpp>
#include <yu/tuples/access/get.hpp>
#include <yu/tuples/access/index.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/projected.hpp>
#include <yu/tuples/utility/index_sequence_for.hpp>
#include <cstddef>
#include <functional>
#include <optional>
#include <utility>

namespace yu::tuples {

namespace _unspecified::find_first_of {

struct fn {
    public:
        template <
            tuple Tuple1,
            tuple Tuple2,
            typename Pred  = functional::safe_equal_to,
            typename Proj1 = std::identity,
            typename Proj2 = std::identity
        >
                static constexpr std::optional<std::size_t> operator()(
            Tuple1&& tuple1,
            Tuple2&& tuple2,
            Pred     pred  = {},
            Proj1    proj1 = {},
            Proj2    proj2 = {}
        ) {
            auto test = [&](auto I) {
                return tuples::any_of(
                    std::forward<Tuple2>(tuple2),
                    std::bind_back(std::ref(pred), std::invoke(proj1, tuples::get(std::forward<Tuple1>(tuple1), I))),
                    std::ref(proj2)
                );
            };

            auto find_index = [&]<std::size_t... Idx>(std::index_sequence<Idx...>) {
                std::optional<std::size_t> result;

                (void)((test(index<Idx>) && (result = Idx, true)) || ...);

                return result;
            };

            return find_index(indices_for<Tuple1>);
        }
};

} // namespace _unspecified::find_first_of

inline namespace _fn {

inline constexpr _unspecified::find_first_of::fn find_first_of;

}

} // namespace yu::tuples

#endif
