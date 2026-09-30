// yutool: include guard
#ifndef YU_TUPLES_VIEWS_REF_VIEW_HPP_
#define YU_TUPLES_VIEWS_REF_VIEW_HPP_

#include "view_interface.hpp"
#include <yu/tuples/access/index.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/type_traits.hpp>
#include <concepts>
#include <cstddef>
#include <memory>
#include <type_traits>

namespace yu::tuples {

template <tuple Tuple>
requires std::is_object_v<Tuple>
class ref_view : public view_interface<ref_view<Tuple>> {
    private:
        Tuple* base_ptr_;

        // Declaration only
        static void bind_lvalue(Tuple&);
        static void bind_lvalue(Tuple&&) = delete;

    public:
        static constexpr size<Tuple> size{};

        template <typename T>
        requires (!std::same_as<std::remove_cvref_t<T>, ref_view>)
                 && std::convertible_to<T, Tuple&> && requires { bind_lvalue(std::declval<T>()); }
        constexpr ref_view(T&& tuple) :
            base_ptr_(std::addressof(static_cast<Tuple&>(std::forward<T>(tuple)))) {}

        [[nodiscard]]
        constexpr Tuple& base() const noexcept {
            return *base_ptr_;
        }

        template <std::size_t Idx, typename Self>
        requires (Idx < size)
        [[nodiscard]]
        constexpr decltype(auto) get(this Self&& self) noexcept(noexcept(tuples::get(self.base(), index<Idx>))) {
            return tuples::get(self.base(), index<Idx>);
        }
};

template <typename Tuple>
ref_view(Tuple&) -> ref_view<Tuple>;

} // namespace yu::tuples

#endif
