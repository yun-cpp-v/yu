// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_ENDS_WITH_HPP_
#define YU_TUPLES_ALGORITHM_ENDS_WITH_HPP_

#include "yu/functional/predicates.hpp"
#include <yu/functional/predicates.hpp>
#include <yu/tuples/access/get.hpp>
#include <yu/tuples/access/size.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/projected.hpp>
#include <yu/tuples/utility/index_sequence_for.hpp>
#include <cstddef>
#include <functional>
#include <utility>

namespace yu::tuples {

namespace _unspecified::ends_with {

struct fn {
    public:
        template <
            tuple Tuple1,
            tuple Tuple2,
            typename Pred  = functional::safe_equal_to,
            typename Proj1 = std::identity,
            typename Proj2 = std::identity
        >
        [[nodiscard]]
        static constexpr bool operator()(
            Tuple1&& tuple1,
            Tuple2&& tuple2,
            Pred     pred  = {},
            Proj1    proj1 = {},
            Proj2    proj2 = {}
        ) {
            constexpr std::size_t size1 = size_v<Tuple1>, size2 = size_v<Tuple2>;

            if constexpr (size1 < size2) {
                return false;
            } else {
                constexpr std::size_t size_diff = size1 - size2;

                auto safe_pred = functional::with_fallback(std::ref(pred), false);

                return [&]<std::size_t... Idx>(std::index_sequence<Idx...>) {
                    return (
                        std::invoke(
                            safe_pred,
                            std::invoke(proj1, tuples::get(std::forward<Tuple1>(tuple1), index<size_diff + Idx>)),
                            std::invoke(proj2, tuples::get(std::forward<Tuple2>(tuple2), index<Idx>))
                        )
                        && ...
                    );
                }(indices_for<Tuple2>);
            }
        }
};

} // namespace _unspecified::ends_with

inline namespace _fn {

inline constexpr _unspecified::ends_with::fn ends_with{};

}

} // namespace yu::tuples

#endif
