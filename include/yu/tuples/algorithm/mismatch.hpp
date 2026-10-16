// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_MISMATCH_HPP_
#define YU_TUPLES_ALGORITHM_MISMATCH_HPP_

#include <yu/functional/predicates.hpp>
#include <yu/functional/with_fallback.hpp>
#include <yu/tuples/access/get.hpp>
#include <yu/tuples/access/index.hpp>
#include <yu/tuples/access/size.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/projected.hpp>
#include <yu/tuples/utility/index_sequence_for.hpp>
#include <algorithm>
#include <cstddef>
#include <functional>
#include <optional>
#include <utility>

namespace yu::tuples {

namespace _unspecified::mismatch {

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
            constexpr std::size_t size1 = size_v<Tuple1>, size2 = size_v<Tuple2>, min_size = std::min(size1, size2);

            auto safe_pred = functional::with_fallback(std::ref(pred), false);

            auto test = [&](auto I) {
                return std::invoke(
                    safe_pred,
                    std::invoke(proj1, tuples::get(std::forward<Tuple1>(tuple1), I)),
                    std::invoke(proj2, tuples::get(std::forward<Tuple2>(tuple2), I))
                );
            };

            auto find_mismatched_index = [&]<std::size_t... Idx>(std::index_sequence<Idx...>) {
                std::optional<std::size_t> result;

                (void)((!test(index<Idx>) && (result = Idx, true)) || ...);

                if (!result.has_value()) {
                    if constexpr (size1 != size2) result = min_size;
                }

                return result;
            };

            return find_mismatched_index(std::make_index_sequence<min_size>{});
        }
};

} // namespace _unspecified::mismatch

inline namespace _fn {

inline constexpr _unspecified::mismatch::fn mismatch{};

}

} // namespace yu::tuples

#endif
