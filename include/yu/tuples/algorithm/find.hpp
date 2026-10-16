// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_FIND_HPP_
#define YU_TUPLES_ALGORITHM_FIND_HPP_

#include "find_if.hpp"
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/projected.hpp>
#include <yu/tuples/utility/index_sequence_for.hpp>
#include <functional>
#include <optional>

namespace yu::tuples {

namespace _unspecified::find {

struct fn {
        template <tuple Tuple, typename T, typename Proj = std::identity>
                static constexpr std::optional<std::size_t> operator()(Tuple&& tuple, const T& value, Proj proj = {}) {
            return tuples::find_if(
                std::forward<Tuple>(tuple),
                std::bind_back(std::ranges::equal_to{}, value),
                std::ref(proj)
            );
        }
};

} // namespace _unspecified::find

inline namespace _fn {

inline constexpr _unspecified::find::fn find{};

}

} // namespace yu::tuples

#endif
