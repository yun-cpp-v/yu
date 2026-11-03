// yutool: include guard
#ifndef YU_UTILITY_FORWARD_LIKE_T_HPP_
#define YU_UTILITY_FORWARD_LIKE_T_HPP_

#include <utility>

namespace yu {

template <typename T, typename U>
using forward_like_t = decltype(std::forward_like<T>(std::declval<U>()));

} // namespace yu

#endif
