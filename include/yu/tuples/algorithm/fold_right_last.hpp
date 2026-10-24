// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_FOLD_RIGHT_LAST_HPP_
#define YU_TUPLES_ALGORITHM_FOLD_RIGHT_LAST_HPP_

#include "_detail/fold_right_impl.hpp"
#include "_detail/right_fold_concepts.hpp"
#include <yu/tuples/access/get.hpp>
#include <yu/tuples/access/index.hpp>
#include <yu/tuples/apply.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/utility/index_sequence_for.hpp>
#include <cstddef>
#include <utility>

namespace yu::tuples {

namespace _unspecified::fold_right_last {

struct fn {
    public:
        template <tuple Tuple, typename Fn>
        requires _detail::right_foldable_last<Fn, Tuple>
        static constexpr auto operator()(Tuple&& tuple, Fn f) {
            constexpr std::size_t size = size_v<Tuple>;

            return [&]<std::size_t... Idx>(std::index_sequence<Idx...>) {
                return _detail::fold_right_impl(f, tuples::get(tuple, index<size - Idx - 1>)...);
            }(indices_for<Tuple>);
        }
};

} // namespace _unspecified::fold_right_last

inline namespace _fn {

inline constexpr _unspecified::fold_right_last::fn fold_right_last;

}

} // namespace yu::tuples

#endif
