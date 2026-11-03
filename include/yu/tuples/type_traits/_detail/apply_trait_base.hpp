// yutool: include guard
#ifndef YU_TUPLES_TYPE_TRAITS_DETAIL_APPLY_TRAIT_BASE_HPP_
#define YU_TUPLES_TYPE_TRAITS_DETAIL_APPLY_TRAIT_BASE_HPP_

#include "yu/tuples/access/size.hpp"
#include <yu/tuples/type_traits/element_type.hpp>
#include <utility>

namespace yu::tuples::_detail {

template <
    typename IndexSequence,
    template <typename...> typename InvocationTrait,
    typename Tup,
    typename Fn,
    typename... Args
>
struct apply_trait_base_impl {};

template <
    std::size_t... Idx,
    template <typename...> typename InvocationTrait,
    typename Tuple,
    typename Fn,
    typename... Args
>
requires tuples::tuple<Tuple> && ((Idx < size_v<Tuple>) && ...)
struct apply_trait_base_impl<std::index_sequence<Idx...>, InvocationTrait, Tuple, Fn, Args...> :
    InvocationTrait<Args..., Fn, element_type_t<Idx, Tuple>...> {};

template <template <typename...> typename InvocationTrait, typename Tuple, typename Fn, typename... Args>
using apply_trait_base
    = apply_trait_base_impl<std::make_index_sequence<size_v<Tuple>>, InvocationTrait, Tuple, Fn, Args...>;

} // namespace yu::tuples::_detail

#endif
