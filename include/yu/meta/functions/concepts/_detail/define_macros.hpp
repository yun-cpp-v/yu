// yutool: include guard
#include <yu/meta/constant.hpp> // IWYU pragma: keep
#include <yu/meta/type.hpp>     // IWYU pragma: keep
#include <concepts>             // IWYU pragma: keep

#ifndef DEFINE_FUNCTION_OBJECT
#define DEFINE_FUNCTION_OBJECT(NAME)                \
    inline namespace _fn {                          \
                                                    \
    inline constexpr _unspecified::NAME::fn NAME; \
                                                    \
    }
#endif

#ifndef DEFINE_UNARY_CONCEPT
#define DEFINE_UNARY_CONCEPT(NAME)                                                             \
    namespace _unspecified::NAME {                                                             \
                                                                                               \
    struct fn {                                                                                \
            template <typename T>                                                              \
            static constexpr auto operator()(type_t<T>) noexcept -> constant_t<std::NAME<T>> { \
                return {};                                                                     \
            }                                                                                  \
    };                                                                                         \
                                                                                               \
    }                                                                                          \
    DEFINE_FUNCTION_OBJECT(NAME)
#endif

#ifndef DEFINE_BINARY_CONCEPT
#define DEFINE_BINARY_CONCEPT(NAME)                                                                          \
    namespace _unspecified::NAME {                                                                           \
                                                                                                             \
    struct fn {                                                                                              \
            template <typename T, typename U>                                                                \
            static constexpr auto operator()(type_t<T>, type_t<U>) noexcept -> constant_t<std::NAME<T, U>> { \
                return {};                                                                                   \
            }                                                                                                \
    };                                                                                                       \
                                                                                                             \
    }                                                                                                        \
    DEFINE_FUNCTION_OBJECT(NAME)
#endif

#ifndef DEFINE_TERNARY_CONCEPT
#define DEFINE_TERNARY_CONCEPT(NAME)                                                   \
    namespace _unspecified::NAME {                                                     \
                                                                                       \
    struct fn {                                                                        \
            template <typename T, typename U, typename V>                              \
            static constexpr auto operator()(type_t<T>, type_t<U>, type_t<V>) noexcept \
                -> constant_t<std::NAME<T, U, V>> {                                    \
                return {};                                                             \
            }                                                                          \
    };                                                                                 \
                                                                                       \
    }                                                                                  \
    DEFINE_FUNCTION_OBJECT(NAME)
#endif

#ifndef DEFINE_AT_LEAST_ONE_ARITY_CONCEPT
#define DEFINE_AT_LEAST_ONE_ARITY_CONCEPT(NAME)                                                                      \
    namespace _unspecified::NAME {                                                                                   \
                                                                                                                     \
    struct fn {                                                                                                      \
            template <typename T, typename... Ts>                                                                    \
            static constexpr auto operator()(type_t<T>, type_t<Ts>...) noexcept -> constant_t<std::NAME<T, Ts...>> { \
                return {};                                                                                           \
            }                                                                                                        \
    };                                                                                                               \
                                                                                                                     \
    }                                                                                                                \
    DEFINE_FUNCTION_OBJECT(NAME)
#endif

#ifndef DEFINE_AT_LEAST_TWO_ARITY_CONCEPT
#define DEFINE_AT_LEAST_TWO_ARITY_CONCEPT(NAME)                                            \
    namespace _unspecified::NAME {                                                         \
                                                                                           \
    struct fn {                                                                            \
            template <typename T, typename U, typename... Ts>                              \
            static constexpr auto operator()(type_t<T>, type_t<U>, type_t<Ts>...) noexcept \
                -> constant_t<std::NAME<T, U, Ts...>> {                                    \
                return {};                                                                 \
            }                                                                              \
    };                                                                                     \
                                                                                           \
    }                                                                                      \
    DEFINE_FUNCTION_OBJECT(NAME)
#endif

#ifndef DEFINE_VARIADIC_CONCEPT
#define DEFINE_VARIADIC_CONCEPT(NAME)                                                                  \
    namespace _unspecified::NAME {                                                                     \
                                                                                                       \
    struct fn {                                                                                        \
            template <typename... Ts>                                                                  \
            static constexpr auto operator()(type_t<Ts>...) noexcept -> constant_t<std::NAME<Ts...>> { \
                return {};                                                                             \
            }                                                                                          \
    };                                                                                                 \
                                                                                                       \
    }                                                                                                  \
    DEFINE_FUNCTION_OBJECT(NAME)

#endif
