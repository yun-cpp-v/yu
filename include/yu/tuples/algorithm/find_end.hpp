// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_FIND_END_HPP_
#define YU_TUPLES_ALGORITHM_FIND_END_HPP_

#include <yu/config/stateless_call_operator.hpp>
#include <yu/functional/predicates.hpp>
#include <yu/functional/with_fallback.hpp>
#include <yu/tuples/access/get.hpp>
#include <yu/tuples/access/size.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/projected.hpp>
#include <yu/tuples/utility/index_sequence_for.hpp>
#include <cstddef>
#include <functional>
#include <optional>
#include <utility>

namespace yu::tuples {

namespace fn {

struct find_end_fn {
        template <
            tuple Tuple1,
            tuple Tuple2,
            typename Pred  = functional::safe_equal_to,
            typename Proj1 = std::identity,
            typename Proj2 = std::identity
        >
        constexpr std::optional<std::size_t> YU_STATELESS_CALL_OPERATOR(
            Tuple1&& tuple1,
            Tuple2&& tuple2,
            Pred     pred  = {},
            Proj1    proj1 = {},
            Proj2    proj2 = {}
        ) {
            constexpr std::size_t size1 = size_v<Tuple1>, size2 = size_v<Tuple2>;

            if constexpr (size1 < size2) {
                return std::nullopt;
            } else {
                auto safe_pred = functional::with_fallback(std::ref(pred), false);

                auto matched_at = [&](auto Offset) {
                    return [&]<std::size_t... Idx>(std::index_sequence<Idx...>) {
                        return (
                            std::invoke(
                                safe_pred,
                                std::invoke(proj1, tuples::get(std::forward<Tuple1>(tuple1), Offset + index<Idx>)),
                                std::invoke(proj2, tuples::get(std::forward<Tuple2>(tuple2), index<Idx>))
                            )
                            && ...
                        );
                    }(indices_for<Tuple2>);
                };

                auto find_matched_offset = [&]<std::size_t... Offsets>(std::index_sequence<Offsets...>) {
                    constexpr std::size_t size_diff = size1 - size2;

                    std::optional<std::size_t> result;

                    (void)((matched_at(index<size_diff - Offsets>) && (result = size_diff - Offsets, true)) || ...);

                    return result;
                };

                return find_matched_offset(std::make_index_sequence<size1 - size2 + 1>{});
            }
        }
};

} // namespace fn

inline namespace _fn {

inline constexpr fn::find_end_fn find_end;

}

} // namespace yu::tuples

#endif
