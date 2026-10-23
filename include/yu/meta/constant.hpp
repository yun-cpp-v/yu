// yutool: include guard
#ifndef YU_META_CONSTANT_HPP_
#define YU_META_CONSTANT_HPP_

#include "concepts/constant_like.hpp"
#include <functional>
#include <type_traits>

namespace yu::meta {

namespace _detail::constant {

struct operators {
    public:
        template <constant_like T, constant_like... Args>
        requires requires { constant_t<T::value(Args::value...)>{}; }
        constexpr auto operator()(this T, Args...) noexcept {
            return constant_t<T::value(Args::value...)>{};
        }

        template <constant_like T, constant_like... Args>
        requires requires { constant_t<T::value[Args::value...]>{}; }
        constexpr auto operator[](this T, Args...) noexcept {
            return constant_t<T::value[Args::value...]>{};
        }

#define DEFINE_UNARY_OPERATOR(OP)                                                \
    template <constant_like T>                                                   \
    friend constexpr auto operator OP(T) noexcept -> constant_t<(OP T::value)> { \
        return {};                                                               \
    }
        DEFINE_UNARY_OPERATOR(+)
        DEFINE_UNARY_OPERATOR(-)
        DEFINE_UNARY_OPERATOR(~)
        DEFINE_UNARY_OPERATOR(!)
#undef DEFINE_UNARY_OPERATOR

#define DEFINE_BINARY_OPERATOR(OP)                                                           \
    template <constant_like L, constant_like R>                                              \
    friend constexpr auto operator OP(L, R) noexcept -> constant_t<(L::value OP R::value)> { \
        return {};                                                                           \
    }
        DEFINE_BINARY_OPERATOR(+)
        DEFINE_BINARY_OPERATOR(-)
        DEFINE_BINARY_OPERATOR(*)
        DEFINE_BINARY_OPERATOR(/)
        DEFINE_BINARY_OPERATOR(%)
        DEFINE_BINARY_OPERATOR(<<)
        DEFINE_BINARY_OPERATOR(>>)
        DEFINE_BINARY_OPERATOR(&)
        DEFINE_BINARY_OPERATOR(|)
        DEFINE_BINARY_OPERATOR(^)

        DEFINE_BINARY_OPERATOR(<)
        DEFINE_BINARY_OPERATOR(<=)
        DEFINE_BINARY_OPERATOR(==)
        DEFINE_BINARY_OPERATOR(!=)
        DEFINE_BINARY_OPERATOR(>)
        DEFINE_BINARY_OPERATOR(>=)
#undef DEFINE_BINARY_OPERATOR

#define DEFINE_BINARY_LOGICAL_OPERATOR(OP)                                                   \
    template <constant_like L, constant_like R>                                              \
    requires (!std::is_constructible_v<bool, decltype(L::value)>)                            \
             || (!std::is_constructible_v<bool, decltype(R::value)>)                         \
    friend constexpr auto operator OP(L, R) noexcept -> constant_t<(L::value OP R::value)> { \
        return {};                                                                           \
    }
        DEFINE_BINARY_LOGICAL_OPERATOR(&&)
        DEFINE_BINARY_LOGICAL_OPERATOR(||)
#undef DEFINE_BINARY_LOGICAL_OPERATOR
};

} // namespace _detail::constant

template <auto X>
struct constant_t : _detail::constant::operators {
        static constexpr auto value = X;
        using type                  = constant_t;
        using value_type            = std::remove_cvref_t<decltype(X)>;
        using integral_constant     = std::integral_constant<value_type, value>;

        constexpr constant_t() noexcept = default;

        template <constant_like T>
        constexpr explicit constant_t(T&&) noexcept {}

        constexpr operator value_type() const noexcept { return value; }
};

template <constant_like F, constant_like... Args>
requires requires { constant_t<std::invoke(F::value, Args::value...)>{}; }
constexpr auto constant_invoke(F, Args...) noexcept {
    return constant_t<std::invoke(F::value, Args::value...)>{};
}

template <constant_like T>
constant_t(T&&) -> constant_t<std::remove_cvref_t<T>::value>;

template <auto V>
inline constexpr constant_t<V> constant;

} // namespace yu::meta

#endif
