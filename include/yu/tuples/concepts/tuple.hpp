// yutool: include guard
#ifndef YU_TUPLES_CONCEPTS_TUPLE_HPP_
#define YU_TUPLES_CONCEPTS_TUPLE_HPP_

#include <yu/tuples/access/get.hpp>
#include <yu/tuples/access/index.hpp>
#include <yu/tuples/access/size.hpp>
#include <yu/utility/with_indices.hpp>
#include <utility>

namespace yu::tuples {

template <typename T>
concept tuple = with_indices<size<std::remove_reference_t<T>>::value>([](auto... idx) {
    return (requires(T&& t) { tuples::get(std::forward<T>(t), idx); } && ...);
});

} // namespace yu::tuples

#endif
