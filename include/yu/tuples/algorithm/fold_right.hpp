// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_FOLD_RIGHT_HPP_
#define YU_TUPLES_ALGORITHM_FOLD_RIGHT_HPP_

#include "_detail/fold_helpers.hpp"
#include <yu/config/stateless_call_operator.hpp>
#include <yu/tuples/apply.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <type_traits>
#include <utility>

namespace yu::tuples {

namespace fn {

struct fold_right_fn {
        template <tuple Tuple, typename T, typename Fn>
        constexpr auto YU_STATELESS_CALL_OPERATOR(Tuple&& tuple, T init, Fn fn) {
            if constexpr (size_v<std::remove_reference_t<Tuple>> == 0) {
                return init;
            } else {
                return apply(
                    [&init, &fn]<typename... Elems>(Elems&&... elems) {
                        auto fn_ref = std::ref(fn);

                        return (_detail::rfold_helper{init} * ...
                                * _detail::rfold_helper{std::forward<Elems>(elems), fn_ref})
                            .stored;
                    },
                    std::forward<Tuple>(tuple)
                );
            }
        }
};

} // namespace fn

inline namespace _fn {

inline constexpr fn::fold_right_fn fold_right;

}

} // namespace yu::tuples

#endif
