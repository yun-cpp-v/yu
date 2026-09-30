// yutool: include guard
#ifndef YU_TUPLES_VIEWS_REPEAT_VIEW_HPP_
#define YU_TUPLES_VIEWS_REPEAT_VIEW_HPP_

#include "view_interface.hpp"
#include <yu/tuples/access/index.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <concepts>
#include <type_traits>
#include <utility>

namespace yu::tuples {

template <std::move_constructible T, std::size_t Count>
requires std::is_object_v<T> && std::same_as<T, std::remove_cv_t<T>>
class repeat_view : public view_interface<repeat_view<T, Count>> {
    private:
        T value_;

    public:
        static constexpr index_t<Count> size{};

        constexpr explicit repeat_view(T value, index_t<Count>) :
            value_(std::move(value)) {}

        template <std::size_t Idx, typename Self>
        requires (Idx < size)
        [[nodiscard]]
        constexpr decltype(auto) get(this Self&& self) noexcept {
            return std::forward_like<Self>(self.value_);
        }
};

template <typename Tuple, std::size_t Size>
repeat_view(Tuple, index_t<Size>) -> repeat_view<Tuple, Size>;

namespace views {

namespace _unspecified::repeat {

struct fn {
        template <std::move_constructible T, std::size_t Size>
        requires std::is_object_v<T>
        [[nodiscard]]
        static constexpr decltype(auto) operator()(T&& value, index_t<Size> index) noexcept(
            noexcept(repeat_view{std::forward<T>(value), index})
        ) {
            return repeat_view{std::forward<T>(value), index};
        }
};

} // namespace _unspecified::repeat

inline constexpr _unspecified::repeat::fn repeat;

} // namespace views

} // namespace yu::tuples

#endif
