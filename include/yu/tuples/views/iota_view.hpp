// yutool: include guard
#ifndef YU_TUPLES_VIEWS_IOTA_VIEW_HPP_
#define YU_TUPLES_VIEWS_IOTA_VIEW_HPP_

#include "view_interface.hpp"
#include <yu/tuples/access/index.hpp>
#include <yu/tuples/type_traits.hpp>
#include <cstddef>

namespace yu::tuples {

template <std::size_t Start, std::size_t End>
requires (Start <= End)
class iota_view : public view_interface<iota_view<Start, End>> {
    public:
        static constexpr index_t<End - Start> size;

        constexpr explicit iota_view() noexcept = default;

        constexpr explicit iota_view(index_t<Start>, index_t<End>) noexcept {}

        template <std::size_t Idx, typename Self>
        requires (Idx < size)
        constexpr index_t<Start + Idx> get(this Self&&) noexcept {
            return;
        }
};

template <std::size_t Start, std::size_t End>
iota_view(index_t<Start>, index_t<End>) -> iota_view<Start, End>;

namespace views {

namespace _unspecified::iota {

struct fn {
        template <std::size_t Start, std::size_t End>
        requires (Start <= End)
        static constexpr auto operator()(index_t<Start>, index_t<End>) noexcept {
            return iota_view<Start, End>{};
        }
};

} // namespace _unspecified::iota

inline constexpr _unspecified::iota::fn iota;

} // namespace views

} // namespace yu::tuples

#endif
