// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_FOR_EACH_N_HPP_
#define YU_TUPLES_ALGORITHM_FOR_EACH_N_HPP_

#include <yu/tuples/access/get.hpp>
#include <yu/tuples/access/index.hpp>
#include <yu/tuples/concepts/elementwise_unary_invocable.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/projected.hpp>
#include <yu/tuples/utility/index_sequence_for.hpp>
#include <cstddef>
#include <functional>
#include <utility>

namespace yu::tuples {

namespace _unspecified::for_each_n {

struct fn {
    public:
        template <tuple Tuple, std::size_t N, typename Fn, typename Proj = std::identity>
        requires elementwise_unary_invocable<Fn, projected<Tuple, Proj>> && (N < size_v<Tuple>)
        static constexpr void operator()(Tuple&& tuple, index_t<N>, Fn fn, Proj proj = {}) {
            auto eval = [&]<std::size_t... Idx>(std::index_sequence<Idx...>) {
                (std::invoke(fn, std::invoke(proj, tuples::get(std::forward<Tuple>(tuple), index<Idx>))), ...);
            };

            eval(std::make_index_sequence<N>{});
        }
};

} // namespace _unspecified::for_each_n

inline namespace _fn {

inline constexpr _unspecified::for_each_n::fn for_each_n;

}
} // namespace yu::tuples

#endif
