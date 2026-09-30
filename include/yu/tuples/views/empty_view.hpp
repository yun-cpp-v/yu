// yutool: include guard
#ifndef YU_TUPLES_VIEWS_EMPTY_VIEW_HPP_
#define YU_TUPLES_VIEWS_EMPTY_VIEW_HPP_

#include "view_interface.hpp"
#include <yu/tuples/access/get.hpp>

namespace yu::tuples {

class empty_view : public view_interface<empty_view> {
    public:
        static constexpr index_t<0> size{};

        constexpr explicit empty_view() noexcept = default;

        template <std::size_t Idx, typename Self>
        constexpr decltype(auto) get(this Self&& self) = delete;
};

namespace views {

inline constexpr empty_view empty{};

}

} // namespace yu::tuples

#endif
