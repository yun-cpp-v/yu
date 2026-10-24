// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_COUNT_HPP_
#define YU_TUPLES_ALGORITHM_COUNT_HPP_

#include "count_if.hpp"
#include <yu/functional/predicates.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/projected.hpp>
#include <cstddef>
#include <functional>
#include <utility>

namespace yu::tuples {

namespace _unspecified::count {

struct fn {
    public:
        template <tuple Tuple, typename T, typename Proj = std::identity>
        static constexpr std::size_t operator()(Tuple&& tuple, const T& value, Proj proj = {}) {
            return tuples::count_if(
                std::forward<Tuple>(tuple),
                std::bind_back(functional::safe_equal_to{}, value),
                std::ref(proj)
            );
        }
};

} // namespace _unspecified::count

inline namespace _fn {

inline constexpr _unspecified::count::fn count;

}

} // namespace yu::tuples

#endif
