// yutool: include guard
#ifndef YU_TUPLES_VIEWS_VIEW_INTERFACE_HPP_
#define YU_TUPLES_VIEWS_VIEW_INTERFACE_HPP_

#include <yu/tuples/access/index.hpp>
#include <yu/utility/constant.hpp>
#include <concepts>
#include <cstddef>
#include <tuple>
#include <type_traits>

namespace yu::tuples {

template <typename D>
requires std::is_class_v<D> && std::same_as<D, std::remove_cv_t<D>>
class view_interface {
    public:
        using derived = D;

        template <std::size_t I, typename View>
        requires std::same_as<std::remove_cvref_t<View>, derived>
        friend constexpr decltype(auto) get(View&& view) noexcept(
            noexcept(std::forward<View>(view).template get<I>())
        ) {
            return std::forward<View>(view).template get<I>();
        }
};

namespace _detail::view_interface {

template <typename T>
concept constant_sized = requires {
    { T::size } -> constant_like_of<std::size_t>;
};

} // namespace _detail::view_interface

} // namespace yu::tuples

namespace std {

template <typename T>
requires derived_from<T, yu::tuples::view_interface<T>> && yu::tuples::_detail::view_interface::constant_sized<T>
struct tuple_size<T> : integral_constant<size_t, decltype(T::size)::value> {};

} // namespace std

#endif
