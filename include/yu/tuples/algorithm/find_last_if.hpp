// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_FIND_LAST_IF_HPP_
#define YU_TUPLES_ALGORITHM_FIND_LAST_IF_HPP_

#include <yu/config/stateless_call_operator.hpp>
#include <yu/functional/with_fallback.hpp>
#include <yu/tuples/access/get.hpp>
#include <yu/tuples/access/index.hpp>
#include <yu/tuples/concepts/elementwise_unary_predicate.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/projected.hpp>
#include <yu/tuples/utility/index_sequence_for.hpp>
#include <cstddef>
#include <functional>
#include <optional>
#include <utility>

namespace yu::tuples {

namespace fn {

struct find_last_if_fn {
        template <tuple Tuple, typename Pred, typename Proj = std::identity>
        constexpr std::optional<std::size_t> YU_STATELESS_CALL_OPERATOR(Tuple&& tuple, Pred pred, Proj proj = {}) {
            constexpr auto size = size_v<Tuple>;

            auto safe_pred = functional::with_fallback(std::ref(pred), false);

            auto test = [&](auto I) {
                return std::invoke(safe_pred, std::invoke(proj, tuples::get(std::forward<Tuple>(tuple), I)));
            };

            auto find_index = [&]<std::size_t... Idx>(std::index_sequence<Idx...>) {
                std::optional<std::size_t> result;

                (void)((test(index<size - Idx - 1>) && (result = size - Idx - 1, true)) || ...);

                return result;
            };

            return find_index(indices_for<Tuple>);
        }
};

} // namespace fn

inline namespace _fn {

inline constexpr fn::find_last_if_fn find_last_if;

}

} // namespace yu::tuples

#endif
