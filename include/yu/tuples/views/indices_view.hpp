// yutool: include guard
#ifndef YU_TUPLES_VIEWS_INDICES_VIEW_HPP_
#define YU_TUPLES_VIEWS_INDICES_VIEW_HPP_

#include "iota_view.hpp"
#include "tuple_adaptor_closure.hpp"
#include <yu/config/constructor_object.hpp>
#include <cstddef>

namespace yu::tuples {

template <std::size_t Size>
using indices_view = iota_view<0, Size>;

namespace views {

inline constexpr adaptor_closure indices = YU_CONSTRUCTOR_OBJECT(indices_view);

} // namespace views

} // namespace yu::tuples

#endif
