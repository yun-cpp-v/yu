// yutool: include guard
#ifndef YU_TUPLES_APPLY_HPP_
#define YU_TUPLES_APPLY_HPP_

#include "access/get.hpp"
#include "concepts/tuple.hpp"
#include "type_traits/apply_result.hpp"
#include "type_traits/is_applicable.hpp"
#include <yu/config/stateless_call_operator.hpp>
#include <yu/utility/with_indices.hpp>
#include <functional>
#include <utility>

namespace yu::tuples {

namespace fn {

struct apply_fn {
        template <typename Fn, tuple Tuple>
        constexpr apply_result_t<Fn, Tuple> YU_STATELESS_CALL_OPERATOR(Fn&& fn, Tuple&& tup) noexcept(
            is_nothrow_applicable_v<Fn, Tuple>
        ) {
            return with_indices<size_v<std::remove_reference_t<Tuple>>>([&](auto... idx) -> decltype(auto) {
                return std::invoke(std::forward<Fn>(fn), get(std::forward<Tuple>(tup), idx)...);
            });
        }
};

} // namespace fn

inline namespace _fn {

inline constexpr fn::apply_fn apply;

}

} // namespace yu::tuples

#endif
