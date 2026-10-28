// yutool: include guard
#ifndef YU_TUPLES_APPLY_HPP_
#define YU_TUPLES_APPLY_HPP_

#include "access/get.hpp"
#include "access/index.hpp"
#include "concepts/tuple.hpp"
#include "type_traits/apply_result.hpp"
#include "type_traits/is_applicable.hpp"
#include "utility.hpp"
#include <yu/config/stateless_call_operator.hpp>
#include <cstddef>
#include <functional>
#include <utility>

namespace yu::tuples {

namespace fn {

struct apply_fn {
        template <typename Fn, tuple Tuple>
        constexpr apply_result_t<Fn, Tuple> YU_STATELESS_CALL_OPERATOR(Fn&& fn, Tuple&& tup) noexcept(
            is_nothrow_applicable_v<Fn, Tuple>
        ) {
            return [&]<std::size_t... Idx>(std::index_sequence<Idx...>) -> decltype(auto) {
                return std::invoke(std::forward<Fn>(fn), tuples::get(std::forward<Tuple>(tup), index<Idx>)...);
            }(indices_for<Tuple>);
        }
};

} // namespace fn

inline namespace _fn {

inline constexpr fn::apply_fn apply;

}

} // namespace yu::tuples

#endif
