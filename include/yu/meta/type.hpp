// yutool: include guard
#ifndef YU_META_TYPE_HPP_
#define YU_META_TYPE_HPP_

#include "constant.hpp"
#include <type_traits>

namespace yu::meta {

template <typename Type>
struct type_t {
        using type = Type;

        template <typename T>
        friend constexpr auto operator==(type_t, type_t<T>) noexcept -> constant_t<std::is_same_v<type, T>> {
            return {};
        }

        template <typename T>
        friend constexpr auto operator!=(type_t, type_t<T>) noexcept -> constant_t<!std::is_same_v<type, T>> {
            return {};
        }
};

template <typename Type>
inline constexpr type_t<Type> type;

namespace _unspecified::as_type {

struct fn {
        template <typename T>
        static constexpr auto operator()(T&&) noexcept -> type_t<std::remove_cvref_t<T>> {
            return {};
        }
};

} // namespace _unspecified::as_type

inline namespace _fn {

inline constexpr _unspecified::as_type::fn as_type;

}

} // namespace yu::meta

#endif
