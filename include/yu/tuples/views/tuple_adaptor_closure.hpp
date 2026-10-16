// yutool: include guard
#ifndef YU_TUPLES_VIEWS_TUPLE_ADAPTOR_CLOSURE_HPP_
#define YU_TUPLES_VIEWS_TUPLE_ADAPTOR_CLOSURE_HPP_

#include <yu/tuples/concepts/tuple.hpp>
#include <concepts>
#include <functional>
#include <type_traits>
#include <utility>

namespace yu::tuples {

template <typename D>
requires std::is_class_v<D> && std::same_as<D, std::remove_cv_t<D>>
struct tuple_adaptor_closure;

namespace _detail {

template <typename Closure1, typename Closure2>
struct composed_closure : tuples::tuple_adaptor_closure<composed_closure<Closure1, Closure2>> {
    private:
        [[no_unique_address]]
        Closure1 closure1_;
        [[no_unique_address]]
        Closure2 closure2_;

        template <typename Self, typename Tuple>
        static consteval bool invocable() {
            return requires(Self&& self, Tuple&& tuple) {
                std::invoke(std::forward<Self>(self).closure2(), std::invoke(std::forward<Self>(self).closure1(), std::forward<Tuple>(tuple)));
            };
        }

        template <typename Self, typename Tuple>
        static consteval bool is_nothrow() {
            return noexcept(std::invoke(
                std::declval<Self>().closure2(),
                std::invoke(std::declval<Self>().closure1(), std::declval<Tuple>())
            ));
        }

        template <typename Self>
        constexpr decltype(auto) closure1(this Self&& self) noexcept {
            return std::forward_like<Self>(self.closure1_);
        }

        template <typename Self>
        constexpr decltype(auto) closure2(this Self&& self) noexcept {
            return std::forward_like<Self>(self.closure2_);
        }

    public:
        constexpr explicit composed_closure(Closure1&& closure1, Closure2&& closure2) :
            closure1_(std::move(closure1)), closure2_(std::move(closure2)) {}

        template <typename Self, tuples::tuple Tuple>
        requires (invocable<Self, Tuple>())
        [[nodiscard]]
        constexpr decltype(auto) operator()(this Self&& self, Tuple&& tuple) noexcept(is_nothrow<self, Tuple>()) {
            return std::invoke(std::forward<Self>(self).closure2(), std::invoke(std::forward<Self>(self).closure1(), std::forward<Tuple>(tuple)));
        }
};

template <typename T1, typename T2>
composed_closure(T1, T2) -> composed_closure<T1, T2>;

template <typename T>
concept adaptor_closure = std::derived_from<std::remove_cvref_t<T>, tuple_adaptor_closure<std::remove_cvref_t<T>>>;

} // namespace _detail

template <typename D>
requires std::is_class_v<D> && std::same_as<D, std::remove_cv_t<D>>
struct tuple_adaptor_closure {
        template <typename Self, typename Closure>
        requires _detail::adaptor_closure<Closure>
        [[nodiscard]]
        constexpr auto operator|(this Self&& self, Closure&& closure) {
            return _detail::composed_closure{std::forward<Self>(self), std::forward<Closure>(closure)};
        }

        template <tuple Tuple, typename Closure>
        requires std::same_as<std::remove_cvref_t<Closure>, D> && std::invocable<Closure, Tuple&&>
        [[nodiscard]]
        friend constexpr decltype(auto) operator|(Tuple&& tuple, Closure&& closure) noexcept(
            std::is_nothrow_invocable_v<Closure, Tuple&&>
        ) {
            return std::invoke(std::forward<Closure>(closure), std::forward<Tuple>(tuple));
        }
};

} // namespace yu::tuples

#endif
