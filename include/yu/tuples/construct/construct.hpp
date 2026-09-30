// yutool: include guard
#ifndef YU_TUPLES_CONSTRUCT_CONSTRUCT_HPP_
#define YU_TUPLES_CONSTRUCT_CONSTRUCT_HPP_

#include <yu/tuples/apply.hpp>
#include <yu/tuples/concepts/constructible_from_elements.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/concepts/view.hpp>
#include <type_traits>

namespace yu::tuples {

namespace _detail::construct {

template <template <typename...> typename RetT, typename Tuple, typename... Args>
struct deduce_from_elements {
    private:
        template <std::size_t... Idx>
        static consteval auto deduce(std::index_sequence<Idx...>)
            -> decltype(RetT{std::declval<element_type_t<Idx, Tuple>>()..., std::declval<Args>()...});

    public:
        using type = std::remove_cvref_t<decltype(deduce(indices_for<Tuple>))>;
};

template <template <typename...> typename Ret, typename Tuple, typename... Args>
using deduce_from_elements_t = deduce_from_elements<Ret, Tuple, Args...>::type;

template <typename Ret, typename Tuple, typename... Args>
constexpr Ret impl(Tuple&& tuple, Args&&... args) {
    return tuples::apply(
        [&]<typename... Elems>(Elems&&... elems) {
            return Ret{std::forward<Elems>(elems)..., std::forward<Args>(args)...};
        },
        std::forward<Tuple>(tuple)
    );
}

} // namespace _detail::construct

template <typename Ret, tuple Tuple, typename... Args>
requires constructible_from_elements<Ret, Tuple, Args...> && (!view<Ret>)
[[nodiscard]]
constexpr Ret construct(Tuple&& tuple, Args&&... args) {
    return _detail::construct::impl<Ret>(std::forward<Tuple>(tuple), std::forward<Args>(args)...);
}

template <template <typename...> typename RetT, tuple Tuple, typename... Args>
[[nodiscard]]
constexpr decltype(auto) construct(Tuple&& tuple, Args&&... args) {
    return _detail::construct::impl<_detail::construct::deduce_from_elements_t<RetT, Tuple, Args...>>(
        std::forward<Tuple>(tuple),
        std::forward<Args>(args)...
    );
}

namespace _unspecified::construct {

template <typename Ret, typename... Args>
class type_adaptor {
    private:
        std::tuple<Args...> args_;
        using args_t = std::tuple<Args...>;

    public:
        constexpr explicit type_adaptor(Args... args) :
            args_(std::move(args)...) {}

        template <tuple Tuple, typename Adaptor>
        requires std::same_as<std::remove_cvref_t<Adaptor>, type_adaptor<Ret, Args...>>
        [[nodiscard]]
        friend constexpr auto operator|(Tuple&& tuple, Adaptor&& adaptor) {
            return tuples::apply(
                [&]<typename... Elems>(Elems&&... elems) {
                    return tuples::construct<Ret>(std::forward<Tuple>(tuple), std::forward<Elems>(elems)...);
                },
                std::forward_like<Adaptor>(adaptor.args_)
            );
        }
};

template <template <typename...> typename RetT, typename... Args>
class template_adaptor {
    private:
        std::tuple<Args...> args_;
        using args_t = std::tuple<Args...>;

    public:
        constexpr explicit template_adaptor(Args... args) :
            args_(std::move(args)...) {}

        template <tuple Tuple, typename Adaptor>
        requires std::same_as<std::remove_cvref_t<Adaptor>, template_adaptor<RetT, Args...>>
        [[nodiscard]]
        friend constexpr auto operator|(Tuple&& tuple, Adaptor&& adaptor) {
            return tuples::apply(
                [&]<typename... Elems>(Elems&&... elems) {
                    return tuples::construct<RetT>(std::forward<Tuple>(tuple), std::forward<Elems>(elems)...);
                },
                std::forward_like<Adaptor>(adaptor.args_)
            );
        }
};

} // namespace _unspecified::construct

template <typename Ret, typename... Args>
[[nodiscard]]
constexpr auto construct(Args&&... args) noexcept {
    return _unspecified::construct::type_adaptor<Ret, std::decay_t<Args>...>{std::forward<Args>(args)...};
}

template <template <typename...> typename RetT, typename... Args>
[[nodiscard]]
constexpr auto construct(Args&&... args) noexcept {
    return _unspecified::construct::template_adaptor<RetT, std::decay_t<Args>...>{std::forward<Args>(args)...};
}

} // namespace yu::tuples

#endif
