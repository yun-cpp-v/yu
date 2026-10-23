#include <yu/meta/constant.hpp> // IWYU pragma: export
#include <yu/meta/type.hpp>     // IWYU pragma: export
#include <type_traits>          // IWYU pragma: export

#ifndef DEFINE_FUNCTION_OBJECT
#define DEFINE_FUNCTION_OBJECT(NAME)                \
    inline namespace fn {                           \
                                                    \
    inline constexpr _unspecified::NAME::fn NAME; \
                                                    \
    }
#endif

#ifndef DEFINE_UNARY_TRAIT
#define DEFINE_UNARY_TRAIT(NAME, POSTFIX, RESULT)                                                   \
    namespace _unspecified::NAME {                                                                  \
                                                                                                    \
    struct fn {                                                                                     \
            template <typename T>                                                                   \
            static constexpr auto operator()(type_t<T>) noexcept -> RESULT<std::NAME##POSTFIX<T>> { \
                return {};                                                                          \
            }                                                                                       \
    };                                                                                              \
                                                                                                    \
    }                                                                                               \
    DEFINE_FUNCTION_OBJECT(NAME)
#endif

#ifndef DEFINE_BINARY_TRAIT
#define DEFINE_BINARY_TRAIT(NAME, POSTFIX, RESULT)                                                                \
    namespace _unspecified::NAME {                                                                                \
                                                                                                                  \
    struct fn {                                                                                                   \
            template <typename T, typename U>                                                                     \
            static constexpr auto operator()(type_t<T>, type_t<U>) noexcept -> RESULT<std::NAME##POSTFIX<T, U>> { \
                return {};                                                                                        \
            }                                                                                                     \
    };                                                                                                            \
                                                                                                                  \
    }                                                                                                             \
    DEFINE_FUNCTION_OBJECT(NAME)
#endif

#ifndef DEFINE_TERNARY_TRAIT
#define DEFINE_TERNARY_TRAIT(NAME, POSTFIX, RESULT)                                    \
    namespace _unspecified::NAME {                                                     \
                                                                                       \
    struct fn {                                                                        \
            template <typename T, typename U, typename V>                              \
            static constexpr auto operator()(type_t<T>, type_t<U>, type_t<V>) noexcept \
                -> RESULT<std::NAME##POSTFIX<T, U, V>> {                               \
                return {};                                                             \
            }                                                                          \
    };                                                                                 \
                                                                                       \
    }                                                                                  \
    DEFINE_FUNCTION_OBJECT(NAME)
#endif

#ifndef DEFINE_AT_LEAST_ONE_ARITY_TRAIT
#define DEFINE_AT_LEAST_ONE_ARITY_TRAIT(NAME, POSTFIX, RESULT)                  \
    namespace _unspecified::NAME {                                              \
                                                                                \
    struct fn {                                                                 \
            template <typename T, typename... Ts>                               \
            static constexpr auto operator()(type_t<T>, type_t<Ts>...) noexcept \
                -> RESULT<std::NAME##POSTFIX<T, Ts...>> {                       \
                return {};                                                      \
            }                                                                   \
    };                                                                          \
                                                                                \
    }                                                                           \
    DEFINE_FUNCTION_OBJECT(NAME)
#endif

#ifndef DEFINE_AT_LEAST_TWO_ARITY_TRAIT
#define DEFINE_AT_LEAST_TWO_ARITY_TRAIT(NAME, POSTFIX, RESULT)                             \
    namespace _unspecified::NAME {                                                         \
                                                                                           \
    struct fn {                                                                            \
            template <typename T, typename U, typename... Ts>                              \
            static constexpr auto operator()(type_t<T>, type_t<U>, type_t<Ts>...) noexcept \
                -> RESULT<std::NAME##POSTFIX<T, U, Ts...>> {                               \
                return {};                                                                 \
            }                                                                              \
    };                                                                                     \
                                                                                           \
    }                                                                                      \
    DEFINE_FUNCTION_OBJECT(NAME)
#endif

#ifndef DEFINE_VARIADIC_TRAIT
#define DEFINE_VARIADIC_TRAIT(NAME, POSTFIX, RESULT)                                                        \
    namespace _unspecified::NAME {                                                                          \
                                                                                                            \
    struct fn {                                                                                             \
            template <typename... Ts>                                                                       \
            static constexpr auto operator()(type_t<Ts>...) noexcept -> RESULT<std::NAME##POSTFIX<Ts...>> { \
                return {};                                                                                  \
            }                                                                                               \
    };                                                                                                      \
                                                                                                            \
    }                                                                                                       \
    DEFINE_FUNCTION_OBJECT(NAME)

#endif
