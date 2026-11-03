// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_FIND_IF_HPP_
#define YU_TUPLES_ALGORITHM_FIND_IF_HPP_

#include <yu/config/stateless_call_operator.hpp>
#include <yu/functional/with_fallback.hpp>
#include <yu/tuples/access/get.hpp>
#include <yu/tuples/access/index.hpp>
#include <yu/tuples/concepts/elementwise_unary_predicate.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/projected.hpp>
#include <yu/utility/with_indices.hpp>
#include <cstddef>
#include <functional>
#include <optional>
#include <utility>

namespace yu::tuples {

namespace fn {

struct find_if_fn {
        template <tuple Tuple, typename Pred, typename Proj = std::identity>
        constexpr std::optional<std::size_t> YU_STATELESS_CALL_OPERATOR(Tuple&& tuple, Pred pred, Proj proj = {}) {
            constexpr std::size_t size = size_v<std::remove_reference_t<Tuple>>;

            auto safe_pred = functional::with_fallback(std::ref(pred), false);

            auto test = [&](auto I) {
                return std::invoke(safe_pred, std::invoke(proj, tuples::get(std::forward<Tuple>(tuple), I)));
            };

            return with_indices<size>([&](auto... idx) {
                std::optional<std::size_t> result;

                (void)((test(idx) && (result = idx, true)) || ...);

                return result;
            });
        }
};

} // namespace fn

inline namespace _fn {

inline constexpr fn::find_if_fn find_if;

}

} // namespace yu::tuples

#endif
