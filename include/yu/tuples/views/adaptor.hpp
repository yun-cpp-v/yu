// yutool: include guard
#ifndef YU_TUPLES_VIEWS_ADAPTOR_HPP_
#define YU_TUPLES_VIEWS_ADAPTOR_HPP_

#include "adaptor_closure.hpp"
#include <concepts>
#include <type_traits>
#include <utility>

namespace yu::tuples {

template <typename Fn>
requires std::is_default_constructible_v<Fn>
struct adaptor {
        constexpr adaptor(const Fn&) {}

        template <typename... Args>
        requires (1 <= sizeof...(Args))
        constexpr auto YU_STATELESS_CALL_OPERATOR(Args&&... args) {
            if constexpr (std::invocable<Fn, Args...>) {
                static_assert(1 != sizeof...(Args), "An unary adaptor should be defined as a adaptor_closure.");
                return Fn{}(std::forward<Args>(args)...);
            } else {
                return adaptor_closure{std::bind_back(Fn{}, std::forward<Args>(args)...)};
            }
        }
};

template <typename Fn>
adaptor(Fn) -> adaptor<Fn>;

} // namespace yu::tuples

#endif
