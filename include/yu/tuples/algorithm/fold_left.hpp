// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_FOLD_LEFT_HPP_
#define YU_TUPLES_ALGORITHM_FOLD_LEFT_HPP_

#include "_detail/fold_left_impl.hpp"
#include "_detail/left_fold_concepts.hpp"
#include <yu/config/stateless_call_operator.hpp>
#include <yu/tuples/apply.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <utility>

namespace yu::tuples {

namespace fn {

struct fold_left_fn {
        template <tuple Tuple, typename T, typename Fn>
        requires _detail::left_foldable<Fn, T, Tuple>
        constexpr auto YU_STATELESS_CALL_OPERATOR(Tuple&& tuple, T init, Fn f) {
            if constexpr (size_v<Tuple> == 0) {
                return init;
            } else {
                return tuples::apply(
                    [&]<typename... Elems>(Elems&&... elems) {
                        return _detail::fold_left_impl(f, init, std::forward<Elems>(elems)...);
                    },
                    std::forward<Tuple>(tuple)
                );
            }
        }
};

} // namespace fn

inline namespace _fn {

inline constexpr fn::fold_left_fn fold_left;

}

} // namespace yu::tuples

#endif
