// yutool: include guard
#ifndef YU_TUPLES_VIEWS_INDICES_VIEW_HPP_
#define YU_TUPLES_VIEWS_INDICES_VIEW_HPP_

#include "adaptor_closure.hpp"
#include "iota_view.hpp"
#include <cstddef>

namespace yu::tuples {

template <std::size_t Size>
using indices_view = iota_view<0, Size>;

namespace views {

inline constexpr adaptor_closure indices = []<std::size_t Size>(index_t<Size>) { return indices_view<Size>{}; };

} // namespace views

} // namespace yu::tuples

#endif
