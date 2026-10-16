// yutool: include guard
#ifndef YU_TUPLES_ACCESS_SIZE_HPP_
#define YU_TUPLES_ACCESS_SIZE_HPP_

#include <cstddef>
#include <tuple>
#include <type_traits>

namespace yu::tuples {

template <typename T>
struct size :
    std::conditional_t<
        std::is_bounded_array_v<std::remove_cv_t<T>>,
        std::extent<std::remove_cv_t<T>>,
        std::tuple_size<T>
    > {};

template <typename T>
inline constexpr std::size_t size_v = size<T>::value;

} // namespace yu::tuples

#endif
