// yutool: include guard
#ifndef YU_TUPLES_VIEWS_SINGLE_VIEW_HPP_
#define YU_TUPLES_VIEWS_SINGLE_VIEW_HPP_

#include "view_interface.hpp"
#include <yu/tuples/access/index.hpp>
#include <concepts>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace yu::tuples {

template <std::move_constructible T>
requires std::is_object_v<T>
class single_view : public view_interface<single_view<T>> {
    private:
        T value_;

    public:
        static constexpr index_t<1> size{};

        constexpr explicit single_view(T value) :
            value_(std::move(value)) {}

        template <std::size_t Idx, typename Self>
        requires (Idx < size)
        constexpr decltype(auto) get(this Self&& self) noexcept {
            return std::forward_like<Self>(self.value_);
        }
};

template <typename T>
single_view(T) -> single_view<T>;

namespace views {

namespace _unspecified::single {

struct fn {
        template <std::move_constructible T>
        requires std::is_object_v<T>
        [[nodiscard]]
        static constexpr decltype(auto) operator()(T&& value) noexcept(noexcept(single_view{std::forward<T>(value)})) {
            return single_view{std::forward<T>(value)};
        }
};

} // namespace _unspecified::single

inline constexpr _unspecified::single::fn single{};

} // namespace views

} // namespace yu::tuples

#endif
