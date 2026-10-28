// yutool: include guard
#ifndef YU_TUPLES_VIEWS_REPEAT_VIEW_HPP_
#define YU_TUPLES_VIEWS_REPEAT_VIEW_HPP_

#include "view_interface.hpp"
#include <yu/config/stateless_call_operator.hpp>
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
        constexpr decltype(auto) get(this Self&& self) noexcept {
            return std::forward_like<Self>(self.value_);
        }
};

template <typename Tuple, std::size_t Size>
repeat_view(Tuple, index_t<Size>) -> repeat_view<Tuple, Size>;

namespace views {

namespace fn {

struct repeat_fn {
        template <std::move_constructible T, std::size_t Size>
        requires std::is_object_v<T>
        constexpr auto YU_STATELESS_CALL_OPERATOR(T&& value, index_t<Size> index)  {
            return repeat_view{std::forward<T>(value), index};
        }
};

} // namespace fn

inline constexpr fn::repeat_fn repeat;

} // namespace views

} // namespace yu::tuples

#endif
