// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_FOLD_LEFT_FIRST_HPP_
#define YU_TUPLES_ALGORITHM_FOLD_LEFT_FIRST_HPP_

#include "_detail/fold_helpers.hpp"
#include <yu/config/stateless_call_operator.hpp>
#include <yu/tuples/apply.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <utility>

namespace yu::tuples {

namespace fn {

struct fold_left_first_fn {
        template <tuple Tuple, typename Fn>
        constexpr auto YU_STATELESS_CALL_OPERATOR(Tuple&& tuple, Fn fn) {
            return apply(
                [&fn]<typename... Elems>(Elems&&... elems) {
                    auto fn_ref = std::ref(fn);

                    return (_detail::lfold_helper{std::forward<Elems>(elems), fn_ref} * ...).stored;
                },
                std::forward<Tuple>(tuple)
            );
        }
};

} // namespace fn

inline namespace _fn {

inline constexpr fn::fold_left_first_fn fold_left_first;

}

} // namespace yu::tuples

#endif
