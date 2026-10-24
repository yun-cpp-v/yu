// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_COUNT_IF_HPP_
#define YU_TUPLES_ALGORITHM_COUNT_IF_HPP_

#include <yu/functional/with_fallback.hpp>
#include <yu/tuples/apply.hpp>
#include <yu/tuples/concepts/elementwise_unary_predicate.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/projected.hpp>
#include <cstddef>
#include <functional>

namespace yu::tuples {

namespace _unspecified::count_if {

struct fn {
    public:
        template <tuple Tuple, typename Pred, typename Proj = std::identity>
        requires elementwise_unary_predicate<functional::with_fallback_t<Pred, bool>, projected<Tuple, Proj>>
        static constexpr std::size_t operator()(Tuple&& tuple, Pred pred, Proj proj = {}) {
            auto safe_pred = functional::with_fallback(std::ref(pred), false);

            return tuples::apply(
                [&]<typename... Elems>(Elems&&... elems) {
                    return ((std::invoke(safe_pred, std::invoke(proj, std::forward<Elems>(elems))) ? 1 : 0) + ... + 0);
                },
                std::forward<Tuple>(tuple)
            );
        }
};

} // namespace _unspecified::count_if

inline namespace _fn {

inline constexpr _unspecified::count_if::fn count_if;

}

} // namespace yu::tuples

#endif
