// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_FOR_EACH_HPP_
#define YU_TUPLES_ALGORITHM_FOR_EACH_HPP_

#include <yu/tuples/access/index.hpp>
#include <yu/tuples/apply.hpp>
#include <yu/tuples/concepts/elementwise_unary_invocable.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/projected.hpp>
#include <yu/tuples/utility/index_sequence_for.hpp>
#include <functional>
#include <utility>

namespace yu::tuples {

namespace _unspecified::for_each {

struct fn {
    public:
        template <tuple Tuple, typename Fn, typename Proj = std::identity>
        requires elementwise_unary_invocable<Fn, projected<Tuple, Proj>>
        static constexpr void operator()(Tuple&& tuple, Fn f, Proj proj = {}) {
            tuples::apply(
                [&]<typename... Elems>(Elems&&... elems) {
                    (void)(std::invoke(f, std::invoke(proj, std::forward<Elems>(elems))), ...);
                },
                std::forward<Tuple>(tuple)
            );
        }
};

} // namespace _unspecified::for_each

inline namespace _fn {

inline constexpr _unspecified::for_each::fn for_each;

}

} // namespace yu::tuples

#endif
