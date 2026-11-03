// yutool: include guard
#ifndef YU_TUPLES_VIEWS_TUPLE_ADAPTOR_CLOSURE_HPP_
#define YU_TUPLES_VIEWS_TUPLE_ADAPTOR_CLOSURE_HPP_

#include <yu/tuples/concepts/view.hpp>
#include <yu/utility/forward_like_t.hpp>
#include <utility>

namespace yu::tuples {

template <typename D>
requires std::is_class_v<D> && std::same_as<D, std::remove_cv_t<D>>
struct tuple_adaptor_closure_base {};

template <typename T>
concept tuple_adaptor_closure
    = std::derived_from<std::remove_cvref_t<T>, tuple_adaptor_closure_base<std::remove_cvref_t<T>>>;

template <typename Fn>
struct adaptor_closure : tuple_adaptor_closure_base<adaptor_closure<Fn>> {
    private:
        [[no_unique_address]]
        Fn fn_;

    public:
        constexpr adaptor_closure(Fn fn) :
            fn_(std::move(fn)) {}

        template <typename Self, view View>
        requires std::invocable<forward_like_t<Self, Fn>, View>
        constexpr decltype(auto) operator()(this Self&& self, View&& view) {
            return std::invoke(std::forward_like<Self>(self.fn_), std::forward<View>(view));
        }
};

template <typename Fn>
adaptor_closure(Fn) -> adaptor_closure<Fn>;

template <tuple Tuple, tuple_adaptor_closure Taco>
requires std::invocable<Taco, Tuple>
constexpr auto operator|(Tuple&& tuple, Taco&& taco) {
    return std::invoke(std::forward<Taco>(taco), std::forward<Tuple>(tuple));
}

template <tuple_adaptor_closure TacoL, tuple_adaptor_closure TacoR>
constexpr auto operator|(TacoL&& taco_left, TacoR&& taco_right) {
    return adaptor_closure{std::bind_back(
        []<tuple Tuple, typename Taco1, typename Taco2>(Tuple&& tuple, Taco1&& taco1, Taco2&& taco2) {
            return std::forward<Tuple>(tuple) | std::forward<Taco1>(taco1) | std::forward<Taco2>(taco2);
        },
        std::forward<TacoL>(taco_left),
        std::forward<TacoR>(taco_right)
    )};
}

template <typename Fn>
requires std::is_default_constructible_v<Fn>
struct adaptor {
        constexpr adaptor(const Fn&) {}

        template <typename... Args>
        requires (1 <= sizeof...(Args))
        constexpr auto YU_STATELESS_CALL_OPERATOR(Args&&... args) {
            if constexpr (std::invocable<Fn, Args...>) {
                static_assert(1 != sizeof...(Args), "An unary adaptor should be defined as a adaptor_closure.");
                return Fn{}(std::forward<Args>(args)...);
            } else {
                return adaptor_closure{std::bind_back(Fn{}, std::forward<Args>(args)...)};
            }
        }
};

template <typename Fn>
adaptor(Fn) -> adaptor<Fn>;

} // namespace yu::tuples

#endif
