// yutool: include guard
#ifndef YU_TUPLES_APPLY_HPP_
#define YU_TUPLES_APPLY_HPP_

#include "access/get.hpp"
#include "access/index.hpp"
#include "concepts/tuple.hpp"
#include "type_traits/apply_result.hpp"
#include "type_traits/is_applicable.hpp"
#include "utility.hpp"
#include <cstddef>
#include <functional>
#include <utility>

namespace yu::tuples {

namespace _unspecified::apply {

struct fn {
    public:
        template <typename Fn, tuple Tuple>
        static constexpr apply_result_t<Fn, Tuple> operator()(Fn&& fn, Tuple&& tup) noexcept(
            is_nothrow_applicable_v<Fn, Tuple>
        ) {
            return [&]<std::size_t... Idx>(std::index_sequence<Idx...>) -> decltype(auto) {
                return std::invoke(std::forward<Fn>(fn), tuples::get(std::forward<Tuple>(tup), index<Idx>)...);
            }(indices_for<Tuple>);
        }
};

} // namespace _unspecified::apply

inline namespace _fn {

inline constexpr _unspecified::apply::fn apply;

}

} // namespace yu::tuples

#endif
