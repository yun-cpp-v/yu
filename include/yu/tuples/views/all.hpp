// yutool: include guard
#ifndef YU_TUPLES_VIEWS_ALL_HPP_
#define YU_TUPLES_VIEWS_ALL_HPP_

#include "owning_view.hpp"
#include "ref_view.hpp"
#include "tuple_adaptor_closure.hpp"
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/concepts/view.hpp>
#include <type_traits>
#include <utility>

namespace yu::tuples {

namespace views {

namespace _detail::all {

template <typename Tuple>
concept ref_viewable = requires(Tuple&& tuple) { ref_view{std::forward<Tuple>(tuple)}; };

template <typename Tuple>
concept owning_viewable = requires(Tuple&& tuple) { owning_view{std::forward<Tuple>(tuple)}; };

template <typename Tuple>
concept view_or_viewable = view<std::remove_cvref_t<Tuple>> || ref_viewable<Tuple> || owning_viewable<Tuple>;

} // namespace _detail::all

namespace _unspecified::all {

struct closure : tuple_adaptor_closure<closure> {
    private:
        template <typename Tuple>
        static consteval bool is_nothrow() {
            if constexpr (view<std::remove_cvref_t<Tuple>>) {
                return noexcept(auto(std::declval<Tuple>()));
            } else if constexpr (_detail::all::ref_viewable<Tuple>) {
                return noexcept(ref_view{std::declval<Tuple>()});
            } else if constexpr (_detail::all::owning_viewable<Tuple>) {
                return noexcept(owning_view{std::declval<Tuple>()});
            }
        }

    public:
        template <tuple Tuple>
        requires _detail::all::view_or_viewable<Tuple>
                static constexpr auto operator()(Tuple&& tuple) noexcept(is_nothrow<Tuple>()) {
            if constexpr (view<std::remove_cvref_t<Tuple>>) {
                return auto(std::forward<Tuple>(tuple));
            } else if constexpr (_detail::all::ref_viewable<Tuple>) {
                return ref_view{std::forward<Tuple>(tuple)};
            } else if constexpr (_detail::all::owning_viewable<Tuple>) {
                return owning_view{std::forward<Tuple>(tuple)};
            }
        }
};

} // namespace _unspecified::all

inline constexpr _unspecified::all::closure all{};

template <tuple Tuple>
using all_t = decltype(all(std::declval<Tuple>()));

} // namespace views

} // namespace yu::tuples

#endif
