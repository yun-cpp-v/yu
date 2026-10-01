// yutool: include guard
#ifndef YU_TUPLES_VIEWS_DROP_VIEW_HPP_
#define YU_TUPLES_VIEWS_DROP_VIEW_HPP_

#include "all.hpp"
#include "partial_closure.hpp"
#include "view_interface.hpp"
#include <yu/tuples/access/index.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/concepts/view.hpp>
#include <yu/tuples/type_traits/element_type.hpp>
#include <algorithm>
#include <cstddef>
#include <utility>

namespace yu::tuples {

template <view View, std::size_t Count>
class drop_view : public view_interface<drop_view<View, Count>> {
    private:
        View base_;

        static constexpr size<View>                           base_size_{};
        static constexpr index_t<std::min(Count, base_size_)> drop_count_{};

    public:
        static constexpr auto size = base_size_ - drop_count_;

        constexpr explicit drop_view(View view) :
            base_(std::move(view)) {}

        template <typename Self>
        [[nodiscard]]
        constexpr decltype(auto) base(this Self&& self) noexcept {
            return std::forward_like<Self>(self.base_);
        }

        template <std::size_t Idx, typename Self>
        requires (Idx < size)
        [[nodiscard]]
        constexpr decltype(auto) get(this Self&& self) noexcept(
            noexcept(tuples::get(self.base(), drop_count_ + index<Idx>))
        ) {
            return tuples::get(self.base(), drop_count_ + index<Idx>);
        }
};

template <typename Tuple, std::size_t Count>
drop_view(Tuple&&, index_t<Count>) -> drop_view<views::all_t<Tuple&&>, Count>;

namespace views {

namespace _unspecified::drop {

struct adaptor {
        template <tuple Tuple, std::size_t Count>
        static constexpr auto operator()(Tuple&& tuple, index_t<Count> count) noexcept(
            noexcept(drop_view{std::forward<Tuple>(tuple), count})
        ) {
            return drop_view{std::forward<Tuple>(tuple), count};
        }

        template <std::size_t Count>
        static constexpr auto operator()(index_t<Count> count) noexcept(
            noexcept(make_partial_closure(adaptor{}, count))
        ) {
            return make_partial_closure(adaptor{}, count);
        }
};

} // namespace _unspecified::drop

inline constexpr _unspecified::drop::adaptor drop{};

} // namespace views

} // namespace yu::tuples

#endif
