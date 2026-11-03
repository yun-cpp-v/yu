// yutool: include guard
#ifndef YU_TUPLES_VIEWS_ALL_HPP_
#define YU_TUPLES_VIEWS_ALL_HPP_

#include "owning_view.hpp"
#include "ref_view.hpp"
#include "tuple_adaptor_closure.hpp"
#include <yu/config/stateless_call_operator.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/concepts/view.hpp>
#include <type_traits>
#include <utility>

namespace yu::tuples {

namespace views {

namespace taco {

struct all_closure : tuple_adaptor_closure_base<all_closure> {
    public:
        template <tuple Tuple>
        requires view<std::remove_cvref_t<Tuple>>
        constexpr auto YU_STATELESS_CALL_OPERATOR(Tuple&& tuple) {
            return auto(std::forward<Tuple>(tuple));
        }

        template <tuple Tuple>
        constexpr auto YU_STATELESS_CALL_OPERATOR(Tuple& tuple) {
            return ref_view{tuple};
        }

        template <tuple Tuple>
        constexpr auto YU_STATELESS_CALL_OPERATOR(Tuple&& tuple) {
            return owning_view{std::move(tuple)};
        }
};

} // namespace taco

inline constexpr taco::all_closure all;

template <tuple Tuple>
using all_t = decltype(all(std::declval<Tuple>()));

} // namespace views

} // namespace yu::tuples

#endif
