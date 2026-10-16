// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_FOLD_LEFT_HPP_
#define YU_TUPLES_ALGORITHM_FOLD_LEFT_HPP_

#include "_detail/fold_left_impl.hpp"
#include "_detail/left_fold_concepts.hpp"
#include <yu/tuples/apply.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <utility>

namespace yu::tuples {

namespace _unspecified::fold_left {

struct fn {
    public:
        template <tuple Tuple, typename T, typename Fn>
        requires _detail::left_foldable<Fn, T, Tuple>
                static constexpr auto operator()(Tuple&& tuple, T init, Fn f) {
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

} // namespace _unspecified::fold_left

inline namespace _fn {

inline constexpr _unspecified::fold_left::fn fold_left{};

}

} // namespace yu::tuples

#endif
