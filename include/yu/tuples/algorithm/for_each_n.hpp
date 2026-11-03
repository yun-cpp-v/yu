// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_FOR_EACH_N_HPP_
#define YU_TUPLES_ALGORITHM_FOR_EACH_N_HPP_

#include <yu/config/stateless_call_operator.hpp>
#include <yu/tuples/access/get.hpp>
#include <yu/tuples/access/index.hpp>
#include <yu/tuples/concepts/elementwise_unary_invocable.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/projected.hpp>
#include <yu/utility/with_indices.hpp>
#include <cstddef>
#include <functional>
#include <type_traits>
#include <utility>

namespace yu::tuples {

namespace fn {

struct for_each_n_fn {
        template <tuple Tuple, std::size_t N, typename Fn, typename Proj = std::identity>
        requires elementwise_unary_invocable<Fn, projected<Tuple, Proj>> && (N < size_v<std::remove_reference_t<Tuple>>)
        constexpr void YU_STATELESS_CALL_OPERATOR(Tuple&& tuple, index_t<N>, Fn fn, Proj proj = {}) {
            return with_indices<N>([&](auto... idx) {
                (std::invoke(fn, std::invoke(proj, tuples::get(std::forward<Tuple>(tuple), idx))), ...);
            });
        }
};

} // namespace fn

inline namespace _fn {

inline constexpr fn::for_each_n_fn for_each_n;

}
} // namespace yu::tuples

#endif
