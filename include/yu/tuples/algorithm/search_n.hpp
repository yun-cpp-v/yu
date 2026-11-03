// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_SEARCH_N_HPP_
#define YU_TUPLES_ALGORITHM_SEARCH_N_HPP_

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
#include <type_traits>
#include <utility>

namespace yu::tuples {

namespace fn {

struct search_n_fn {
        template <
            tuple Tuple,
            typename T,
            std::size_t Count,
            typename Pred = functional::safe_equal_to,
            typename Proj = std::identity
        >
        constexpr std::optional<std::size_t> YU_STATELESS_CALL_OPERATOR(
            Tuple&&  tuple,
            const T& value,
            index_t<Count>,
            Pred pred = {},
            Proj proj = {}
        ) {
            constexpr std::size_t size = size_v<std::remove_reference_t<Tuple>>;

            if constexpr (size < Count) {
                return std::nullopt;
            } else {
                auto safe_pred = functional::with_fallback(std::ref(pred), false);

                auto matched_at = [&](auto offset) {
                    return with_indices<Count>([&](auto... idx) {
                        return (
                            std::invoke(
                                safe_pred,
                                std::invoke(proj, tuples::get(std::forward<Tuple>(tuple), offset + idx)),
                                value
                            )
                            && ...
                        );
                    });
                };

                return with_indices<size - Count + 1>([&](auto... idx) {
                    std::optional<std::size_t> result;

                    (void)((matched_at(idx) && (result = std::size_t(idx), true)) || ...);

                    return result;
                });
            }
        }
};

} // namespace fn

inline namespace _fn {

inline constexpr fn::search_n_fn search_n;

}

} // namespace yu::tuples

#endif
