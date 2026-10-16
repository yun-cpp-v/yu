// yutool: include guard
#ifndef YU_TUPLES_VIEWS_TUPLE_ADAPTOR_CLOSURE2_HPP_
#define YU_TUPLES_VIEWS_TUPLE_ADAPTOR_CLOSURE2_HPP_

#include <yu/tuples/apply.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <concepts>
#include <tuple>
#include <type_traits>
#include <utility>

namespace yu::tuples::experimental {

struct tuple_adaptor_closure;

namespace _detail {

template <typename T>
concept closure = std::derived_from<std::remove_cvref_t<T>, tuple_adaptor_closure>;

template <closure... Closures>
struct composed_closure;

template <typename... Closures>
composed_closure(Closures...) -> composed_closure<Closures...>;

} // namespace _detail

struct tuple_adaptor_closure {
        template <_detail::closure Left, _detail::closure Right>
        friend constexpr auto operator|(Left&& left, Right&& right) {
            return _detail::composed_closure{std::forward<Left>(left), std::forward<Right>(right)};
        }

        template <tuple Tuple, _detail::closure Closure>
        friend constexpr decltype(auto) operator|(Tuple&& tuple, Closure&& closure) noexcept(
            noexcept(std::forward<Closure>(closure)(std::forward<Tuple>(tuple)))
        ) {
            return std::forward<Closure>(closure)(std::forward<Tuple>(tuple));
        }
};

namespace _detail {

template <typename T>
struct is_composed : std::false_type {};

template <typename... Closures>
struct is_composed<composed_closure<Closures...>> : std::true_type {};

template <typename T>
concept composed = is_composed<std::remove_cvref_t<T>>::value;

template <closure... Closures>
struct composed_closure : tuple_adaptor_closure {
        template <closure...>
        friend struct composed_closure;

    private:
        std::tuple<Closures...> closures;

        template <std::size_t I, typename Self, typename Tuple>
        requires (I == sizeof...(Closures))
        constexpr decltype(auto) impl(this Self&&, Tuple&& tuple) noexcept {
            return std::forward<Tuple>(tuple);
        }

        template <std::size_t I, typename Self, typename Tuple>
        constexpr decltype(auto) impl(this Self&& self, Tuple&& tuple) noexcept(
            noexcept(std::forward<Self>(self).template impl<I + 1>(
                std::get<I>(std::forward_like<Self>(self.closures))(std::forward<Tuple>(tuple))
            ))
        ) {
            return std::forward<Self>(self).template impl<I + 1>(
                std::get<I>(std::forward_like<Self>(self.closures))(std::forward<Tuple>(tuple))
            );
        }

    public:
        constexpr explicit composed_closure(Closures... closures) :
            closures(std::move(closures)...) {}

        template <typename Self, tuples::tuple Tuple>
        constexpr decltype(auto) operator()(this Self&& self, Tuple&& tuple) noexcept(
            noexcept(std::forward<Self>(self).template impl<0>(std::forward<Tuple>(tuple)))
        ) {
            return std::forward<Self>(self).template impl<0>(std::forward<Tuple>(tuple));
        }

        template <closure Left, closure Right>
        requires composed<Left> || composed<Right>
        friend constexpr auto operator|(Left&& left, Right&& right) {
            if constexpr (composed<Left> && composed<Right>) {
                return apply(
                    [&]<typename... Cs>(Cs&&... cs) { return composed_closure{std::forward<Cs>(cs)...}; },
                    std::tuple_cat(std::forward_like<Left>(left.closures), std::forward_like<Right>(right.closures))
                );
            } else if constexpr (composed<Left>) {
                return apply(
                    [&]<typename... Cs>(Cs&&... cs) {
                        return composed_closure{std::forward<Cs>(cs)..., std::forward<Right>(right)};
                    },
                    std::forward_like<Left>(left.closures)
                );
            } else if constexpr (composed<Right>) {
                return apply(
                    [&]<typename... Cs>(Cs&&... cs) {
                        return composed_closure{std::forward<Left>(left), std::forward<Cs>(cs)...};
                    },
                    std::forward_like<Right>(right.closures)
                );
            }
        }
};

} // namespace _detail

} // namespace yu::tuples::experimental

#endif
