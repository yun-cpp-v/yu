// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_FOLD_LEFT_HPP_
#define YU_TUPLES_ALGORITHM_FOLD_LEFT_HPP_

#include "_detail/fold_helpers.hpp"
#include <yu/config/stateless_call_operator.hpp>
#include <yu/tuples/apply.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <utility>

namespace yu::tuples {

namespace fn {

struct fold_left_fn {
        template <tuple Tuple, typename T, typename Fn>
        constexpr auto YU_STATELESS_CALL_OPERATOR(Tuple&& tuple, T init, Fn fn) {
            if constexpr (size_v<std::remove_reference_t<Tuple>> == 0) {
                return init;
            } else {
                return apply(
                    [&init, &fn]<typename... Elems>(Elems&&... elems) {
                        auto fn_ref = std::ref(fn);

                        return (_detail::lfold_helper{std::forward<Elems>(elems), fn_ref} * ...
                                * _detail::lfold_helper{init})
                            .stored;
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
