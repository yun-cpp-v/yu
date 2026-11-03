// yutool: include guard
#ifndef YU_FUNCTIONAL_WITH_FALLBACK_HPP_
#define YU_FUNCTIONAL_WITH_FALLBACK_HPP_

#include <yu/utility/forward_like_t.hpp>
#include <concepts>
#include <functional>
#include <utility>

namespace yu::functional {

template <typename Fn, typename T>
struct with_fallback_t {
    private:
        Fn function_;
        T  fallback_;

    public:
        constexpr with_fallback_t() requires std::default_initializable<Fn> && std::default_initializable<T>
        = default;

        constexpr explicit with_fallback_t(Fn function, T fallback) noexcept :
            function_(std::move(function)), fallback_(std::move(fallback)) {}

        template <typename Self, typename... Args>
        requires std::invocable<forward_like_t<Self, Fn>, Args&&...>
        constexpr decltype(auto) operator()(this Self&& self, Args&&... args) noexcept(
            std::is_nothrow_invocable_v<forward_like_t<Self, Fn>, Args&&...>
        ) {
            return std::invoke(std::forward_like<Self>(self.function_), std::forward<Args>(args)...);
        }

        template <typename Self, typename... Args>
        constexpr decltype(auto) operator()(this Self&& self, Args&&...) noexcept {
            return std::forward_like<Self>(self.fallback_);
        }
};

template <typename Fn, typename T>
with_fallback_t(Fn, T) -> with_fallback_t<Fn, T>;

template <typename Fn, typename T>
constexpr auto with_fallback(Fn&& function, T&& fallback) {
    return with_fallback_t{std::forward<Fn>(function), std::forward<T>(fallback)};
}

} // namespace yu::functional

#endif