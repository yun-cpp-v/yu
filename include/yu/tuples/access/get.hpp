// yutool: include guard
#ifndef YU_TUPLES_ACCESS_GET_HPP_
#define YU_TUPLES_ACCESS_GET_HPP_

#include "index.hpp"
#include <cstddef>
#include <utility>

namespace yu::tuples {

namespace _unspecified::get {

// Poison Pill
template <std::size_t>
void get() = delete;

struct fn {
    public:
        template <std::size_t Idx, typename T, std::size_t N>
        static constexpr auto operator()(T (&t)[N], index_t<Idx>) noexcept -> T& {
            return t[Idx];
        }

        template <std::size_t Idx, typename T, std::size_t N>
        static constexpr auto operator()(T (&&t)[N], index_t<Idx>) noexcept -> T&& {
            return std::move(t[Idx]);
        }

        template <std::size_t Idx, typename T>
        requires requires(T&& t) { std::forward<T>(t).template get<Idx>(); }
        static constexpr auto operator()(T&& t, index_t<Idx>) noexcept(noexcept(std::forward<T>(t).template get<Idx>()))
            -> decltype(std::forward<T>(t).template get<Idx>()) {
            return std::forward<T>(t).template get<Idx>();
        }

        template <std::size_t Idx, typename T>
        static constexpr auto operator()(T&& t, index_t<Idx>) noexcept(noexcept(get<Idx>(std::forward<T>(t))))
            -> decltype(get<Idx>(std::forward<T>(t))) {
            return get<Idx>(std::forward<T>(t));
        }
};

} // namespace _unspecified::get

inline namespace _cpo {

inline constexpr _unspecified::get::fn get{};

}

} // namespace yu::tuples

#endif
