// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_SEARCH_HPP_
#define YU_TUPLES_ALGORITHM_SEARCH_HPP_

#include <yu/config/stateless_call_operator.hpp>
#include <yu/functional/predicates.hpp>
#include <yu/functional/with_fallback.hpp>
#include <yu/tuples/access/get.hpp>
#include <yu/tuples/access/index.hpp>
#include <yu/tuples/access/size.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/projected.hpp>
#include <yu/utility/with_indices.hpp>
#include <cstddef>
#include <functional>
#include <optional>
#include <utility>

namespace yu::tuples {

namespace fn {

struct search_fn {
        template <
            tuple Tuple1,
            tuple Tuple2,
            typename Pred  = functional::safe_equal_to,
            typename Proj1 = std::identity,
            typename Proj2 = std::identity
        >
        constexpr std::optional<std::size_t> YU_STATELESS_CALL_OPERATOR(
            Tuple1&& tuple1,
            Tuple2&& tuple2,
            Pred     pred  = {},
            Proj1    proj1 = {},
            Proj2    proj2 = {}
        ) {
            constexpr std::size_t size1 = size_v<std::remove_reference_t<Tuple1>>;
            constexpr std::size_t size2 = size_v<std::remove_reference_t<Tuple2>>;

            if constexpr (size1 < size2) {
                return std::nullopt;
            } else {
                auto safe_pred = functional::with_fallback(std::ref(pred), false);

                auto matched_at = [&](auto offset) {
                    return with_indices<size2>([&](auto... idx) {
                        return (
                            std::invoke(
                                safe_pred,
                                std::invoke(
                                    proj1,
                                    tuples::get(std::forward<Tuple1>(tuple1), offset + idx),
                                    std::invoke(proj2, tuples::get(std::forward<Tuple2>(tuple2), idx))
                                )
                            )
                            && ...
                        );
                    });
                };
            };

            return with_indices<size1 - size2 + 2>([&](auto... offset) {
                std::optional<std::size_t> result;

                (void)((matched_at(offset) && (result = std::size_t(offset), true)) || ...);

                return result;
            });
        }
};

} // namespace fn

inline namespace _fn {

inline constexpr fn::search_fn search;

}

} // namespace yu::tuples

#endif
