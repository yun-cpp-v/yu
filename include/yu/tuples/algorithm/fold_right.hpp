// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_FOLD_RIGHT_HPP_
#define YU_TUPLES_ALGORITHM_FOLD_RIGHT_HPP_

#include "_detail/fold_right_impl.hpp"
#include "_detail/right_fold_concepts.hpp"
#include <yu/config/stateless_call_operator.hpp>
#include <yu/tuples/access/get.hpp>
#include <yu/tuples/access/index.hpp>
#include <yu/tuples/apply.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/utility/index_sequence_for.hpp>
#include <cstddef>
#include <utility>

namespace yu::tuples {

namespace fn {

struct fold_right_fn {
        template <tuple Tuple, typename T, typename Fn>
        requires _detail::right_foldable<Fn, T, Tuple>
        constexpr auto YU_STATELESS_CALL_OPERATOR(Tuple&& tuple, T init, Fn fn) {
            constexpr std::size_t size = size_v<Tuple>;

            if constexpr (size == 0) {
                return init;
            } else {
                return [&]<std::size_t... Idx>(std::index_sequence<Idx...>) {
                    return _detail::fold_right_impl(fn, tuples::get(tuple, index<size - Idx - 1>)..., init);
                }(indices_for<Tuple>);
            }
        }
};

} // namespace fn

inline namespace _fn {

inline constexpr fn::fold_right_fn fold_right;

}

} // namespace yu::tuples

#endif
