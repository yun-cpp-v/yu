// yutool: include guard
#ifndef YU_TUPLES_VIEWS_OWNING_VIEW_HPP_
#define YU_TUPLES_VIEWS_OWNING_VIEW_HPP_

#include "view_interface.hpp"
#include <yu/tuples/access/index.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <concepts>
#include <cstddef>

namespace yu::tuples {

template <tuple Tuple>
requires std::movable<Tuple>
class owning_view : public view_interface<owning_view<Tuple>> {
    private:
        Tuple base_;

        template <typename Self>
        constexpr decltype(auto) base(this Self&& self) noexcept {
            return std::forward_like<Self>(self.base_);
        }

    public:
        static constexpr size<Tuple> size{};

        constexpr owning_view(Tuple&& tuple) :
            base_(std::move(tuple)) {}

        template <std::size_t Idx, typename Self>
        requires (Idx < size)
        [[nodiscard]]
        constexpr decltype(auto) get(this Self&& self) noexcept(noexcept(tuples::get(self.base(), index<Idx>))) {
            return tuples::get(self.base(), index<Idx>);
        }
};

template <typename Tuple>
owning_view(Tuple) -> owning_view<Tuple>;

} // namespace yu::tuples

#endif
