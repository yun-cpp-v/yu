// yutool: include guard
#ifndef YU_TUPLES_VIEWS_PARTIAL_CLOSURE_HPP_
#define YU_TUPLES_VIEWS_PARTIAL_CLOSURE_HPP_

#include "tuple_adaptor_closure.hpp"
#include "yu/tuples/access/get.hpp"
#include <yu/tuples/apply.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

namespace yu::tuples {

template <typename Adaptor, typename... Args>
struct partial_closure : tuple_adaptor_closure<partial_closure<Adaptor, Args...>> {
    private:
        [[no_unique_address]]
        Adaptor             adaptor_;
        std::tuple<Args...> args_;
        using args_t = std::tuple<Args...>;

        template <typename Self>
        constexpr decltype(auto) adaptor(this Self&& self) noexcept {
            return std::forward_like<Self>(self.adaptor_);
        }

        template <typename Self>
        constexpr decltype(auto) args(this Self&& self) noexcept {
            return std::forward_like<Self>(self.args_);
        }

        template <typename Self, std::size_t Idx>
        constexpr decltype(auto) arg(this Self&& self, index_t<Idx> index) noexcept(
            noexcept(tuples::get(std::forward<Self>(self).args(), index))
        ) {
            return tuples::get(std::forward<Self>(self).args(), index);
        }

        template <typename Self, typename Tuple>
        static consteval bool invocable() {
            return []<std::size_t... Idx>(std::index_sequence<Idx...>) {
                return requires(Self&& self, Tuple&& tuple) {
                    std::invoke(
                        std::forward<Self>(self).adaptor(),
                        std::forward<Tuple>(tuple),
                        std::forward<Self>(self).arg(index<Idx>)...
                    );
                };
            }(std::index_sequence_for<Args...>{});
        }

        template <typename Self, typename Tuple>
        static consteval bool is_nothrow() {
            return [&]<std::size_t... Idx>(std::index_sequence<Idx...>) {
                return noexcept(std::invoke(
                    std::declval<Self>().adaptor(),
                    std::declval<Tuple>(),
                    std::declval<Self>().arg(index<Idx>)...
                ));
            }(std::index_sequence_for<Args...>{});
        }

    public:
        constexpr explicit partial_closure(Adaptor adaptor, Args... args) noexcept(
            std::is_nothrow_move_constructible_v<Adaptor> && std::is_nothrow_constructible_v<args_t, Args&&...>
        ) :
            adaptor_(std::move(adaptor)), args_(std::move(args)...) {}

        template <typename Self, tuple Tuple>
        requires (invocable<Self, Tuple>())
        constexpr decltype(auto) operator()(this Self&& self, Tuple&& tuple) noexcept(is_nothrow<Self, Tuple>()) {
            return [&]<std::size_t... Idx>(std::index_sequence<Idx...>) {
                return std::invoke(
                    std::forward<Self>(self).adaptor(),
                    std::forward<Tuple>(tuple),
                    std::forward<Self>(self).arg(index<Idx>)...
                );
            }(std::index_sequence_for<Args...>{});
        }
};

template <typename Adaptor, typename... Args>
partial_closure(Adaptor, Args...) -> partial_closure<Adaptor, Args...>;

template <typename Adaptor, typename... Args>
constexpr auto make_partial_closure(Adaptor&& adaptor, Args&&... args) noexcept(
    noexcept(partial_closure{std::forward<Adaptor>(adaptor), std::forward<Args>(args)...})
) {
    return partial_closure{std::forward<Adaptor>(adaptor), std::forward<Args>(args)...};
}

} // namespace yu::tuples

#endif
