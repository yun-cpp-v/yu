// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_ALL_OF_HPP_
#define YU_TUPLES_ALGORITHM_ALL_OF_HPP_

#include <yu/tuples/apply.hpp>
#include <yu/tuples/concepts/elementwise_unary_predicate.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/projected.hpp>
#include <functional>
#include <utility>

namespace yu::tuples {

namespace _unspecified::all_of {

struct fn {
    public:
        template <tuple Tuple, typename Pred, typename Proj = std::identity>
        requires elementwise_unary_predicate<Pred, projected<Tuple, Proj>>
                static constexpr bool operator()(Tuple&& tuple, Pred pred, Proj proj = {}) {
            return tuples::apply(
                [&]<typename... Elems>(Elems&&... elems) {
                    return (std::invoke(pred, std::invoke(proj, std::forward<Elems>(elems))) && ...);
                },
                std::forward<Tuple>(tuple)
            );
        }
};

} // namespace _unspecified::all_of

inline namespace _fn {

inline constexpr _unspecified::all_of::fn all_of;

}

} // namespace yu::tuples

#endif
