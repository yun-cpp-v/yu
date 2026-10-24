// yutool: include guard
#ifndef YU_META_FUNCTIONS_TYPE_TRAITS_EXTENT_HPP_
#define YU_META_FUNCTIONS_TYPE_TRAITS_EXTENT_HPP_

#include "_detail/define_macros.hpp" // IWYU pragma: keep

namespace yu::meta::functions {

namespace _unspecified::extent {

struct fn {
        template <typename T, unsigned I = 0>
        static constexpr auto operator()(type_t<T>, constant_t<I>) noexcept -> constant_t<std::extent_v<T, I>> {
            return {};
        }
};

} // namespace _unspecified::extent

DEFINE_FUNCTION_OBJECT(extent)

} // namespace yu::meta::functions

#include "_detail/undef_macros.hpp" // IWYU pragma: keep

#endif
