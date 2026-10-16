// yutool: include guard
#ifndef YU_TUPLES_ALGORITHM_SEARCH_N_HPP_
#define YU_TUPLES_ALGORITHM_SEARCH_N_HPP_

#include <yu/functional/predicates.hpp>
#include <yu/functional/with_fallback.hpp>
#include <yu/tuples/access/get.hpp>
#include <yu/tuples/access/index.hpp>
#include <yu/tuples/access/size.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/projected.hpp>
#include <yu/tuples/utility/index_sequence_for.hpp>
#include <cstddef>
#include <functional>
#include <optional>
#include <utility>

namespace yu::tuples {

namespace _unspecified::search_n {

struct fn {
    public:
        template <
            tuple Tuple,
            typename T,
            std::size_t Count,
            typename Pred = functional::safe_equal_to,
            typename Proj = std::identity
        >
                static constexpr std::optional<std::size_t> operator()(
            Tuple&&  tuple,
            const T& value,
            index_t<Count>,
            Pred pred = {},
            Proj proj = {}
        ) {
            constexpr std::size_t size = size_v<Tuple>;

            if constexpr (size < Count) {
                return std::nullopt;
            } else {
                auto safe_pred = functional::with_fallback(std::ref(pred), false);

                auto matched_at = [&](auto Offset) {
                    return [&]<std::size_t... Idx>(std::index_sequence<Idx...>) {
                        return (
                            std::invoke(
                                safe_pred,
                                std::invoke(proj, tuples::get(std::forward<Tuple>(tuple), Offset + index<Idx>)),
                                value
                            )
                            && ...
                        );
                    }(std::make_index_sequence<Count>{});
                };

                auto find_matched_offset = [&]<std::size_t... Offsets>(std::index_sequence<Offsets...>) {
                    std::optional<std::size_t> result;

                    (void)((matched_at(index<Offsets>) && (result = Offsets, true)) || ...);

                    return result;
                };

                return find_matched_offset(std::make_index_sequence<size - Count + 1>{});
            }
        }
};

} // namespace _unspecified::search_n

inline namespace _fn {

inline constexpr _unspecified::search_n::fn search_n{};

}

} // namespace yu::tuples

#endif
