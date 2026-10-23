// yutool: include guard
#ifndef YU_TUPLES_CONSTRUCT_FORWARD_AS_TUPLE_HPP_
#define YU_TUPLES_CONSTRUCT_FORWARD_AS_TUPLE_HPP_

#include <yu/tuples/apply.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <tuple>
#include <utility>

namespace yu::tuples {

namespace _unspecified::forward_as_tuple {

struct adaptor {
        template <tuple Tuple>
                static constexpr decltype(auto) operator()(Tuple&& tuple) noexcept {
            return tuples::apply(
                []<typename... Elems>(Elems&&... elems) {
                    return std::forward_as_tuple(std::forward<Elems>(elems)...);
                },
                std::forward<Tuple>(tuple)
            );
        }

        template <tuple Tuple>
                friend constexpr decltype(auto) operator|(Tuple&& tuple, adaptor) noexcept {
            return operator()(std::forward<Tuple>(tuple));
        }
};

} // namespace _unspecified::forward_as_tuple

inline constexpr _unspecified::forward_as_tuple::adaptor forward_as_tuple;

} // namespace yu::tuples

#endif
