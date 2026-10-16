// yutool: include guard
#ifndef YU_TUPLES_ACCESS_GET_HPP_
#define YU_TUPLES_ACCESS_GET_HPP_

#include "index.hpp"
#include <cstddef>
#include <type_traits>
#include <utility>

namespace yu::tuples {

namespace _detail::get {

// Poison Pill
template <std::size_t>
void get() = delete;

template <std::size_t Idx, typename T>
concept member_gettable = requires(T&& t) { std::forward<T>(t).template get<Idx>(); };

template <std::size_t Idx, typename T>
concept unqualified_gettable = requires(T&& t) { get<Idx>(std::forward<T>(t)); };

template <std::size_t Idx, typename T>
concept gettable
    = std::is_bounded_array_v<std::remove_cvref_t<T>> || member_gettable<Idx, T> || unqualified_gettable<Idx, T>;

} // namespace _detail::get

namespace _unspecified::get {

struct fn {
    private:
        template <std::size_t Idx, typename T>
        static consteval bool is_nothrow() {
            if constexpr (std::is_bounded_array_v<std::remove_cvref_t<T>>) {
                return true;
            } else if constexpr (_detail::get::member_gettable<Idx, T>) {
                return noexcept(std::declval<T>().template get<Idx>());
            } else {
                using _detail::get::get;

                return noexcept(get<Idx>(std::declval<T>()));
            }
        }

    public:
        template <std::size_t Idx, typename T>
        requires _detail::get::gettable<Idx, T>
                static constexpr decltype(auto) operator()(T&& t, index_t<Idx>) noexcept(is_nothrow<Idx, T>()) {
            if constexpr (std::is_bounded_array_v<std::remove_cvref_t<T>>) {
                return std::forward<T>(t)[Idx];
            } else if constexpr (_detail::get::member_gettable<Idx, T>) {
                return std::forward<T>(t).template get<Idx>();
            } else {
                using _detail::get::get;

                return get<Idx>(std::forward<T>(t));
            }
        }
};

} // namespace _unspecified::get

inline namespace _cpo {

inline constexpr _unspecified::get::fn get{};

}

} // namespace yu::tuples

#endif
