// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_FIND_LAST_HPP_
#define YU_TUPLES_ALGORITHM_FIND_LAST_HPP_

#include "find_last_if.hpp"
#include <yu/functional/with_fallback.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/projected.hpp>
#include <yu/tuples/utility/index_sequence_for.hpp>
#include <functional>
#include <optional>

namespace yu::tuples {

namespace fn {

struct find_last_fn {
        template <tuple Tuple, typename T, typename Proj = std::identity>
        constexpr std::optional<std::size_t> YU_STATELESS_CALL_OPERATOR(Tuple&& tuple, const T& value, Proj proj = {}) {
            return tuples::find_last_if(
                std::forward<Tuple>(tuple),
                std::bind_back(std::ranges::equal_to{}, value),
                std::ref(proj)
            );
        }
};

} // namespace fn

inline namespace _fn {

inline constexpr fn::find_last_fn find_last;

}

} // namespace yu::tuples

#endif
