// yutool: include guard
#ifndef YU_TUPLES_VIEWS_TAKE_VIEW_HPP_
#define YU_TUPLES_VIEWS_TAKE_VIEW_HPP_

#include "all.hpp"
#include "partial_closure.hpp"
#include "view_interface.hpp"
#include <yu/tuples/access/index.hpp>
#include <yu/tuples/concepts/tuple.hpp>
#include <yu/tuples/concepts/view.hpp>
#include <yu/tuples/type_traits/element_type.hpp>
#include <cstddef>
#include <utility>

namespace yu::tuples {

template <view View, std::size_t Count>
class take_view : public view_interface<take_view<View, Count>> {
    private:
        static constexpr auto base_size_  = tuples::size<View>{};
        static constexpr auto take_count_ = index<(base_size_ < Count ? base_size_ : Count)>;

        View base_;

        template <typename Self>
        constexpr decltype(auto) base(this Self&& self) noexcept {
            return std::forward_like<Self>(self.base_);
        }

    public:
        static constexpr auto size = take_count_;

        constexpr explicit take_view(View view) :
            base_(std::move(view)) {}

        template <std::size_t Idx, typename Self>
        requires (Idx < size)
        [[nodiscard]]
        constexpr decltype(auto) get(this Self&& self) noexcept(noexcept(tuples::get(self.base(), index<Idx>))) {
            return tuples::get(self.base(), index<Idx>);
        }
};

template <typename Tuple, std::size_t Count>
take_view(Tuple&&, index_t<Count>) -> take_view<views::all_t<Tuple&&>, Count>;

namespace views {

namespace _unspecified::take {

struct adaptor {
        template <tuple Tuple, std::size_t Count>
        static constexpr auto operator()(Tuple&& tuple, index_t<Count> count) noexcept(
            noexcept(take_view{std::forward<Tuple>(tuple), count})
        ) {
            return take_view{std::forward<Tuple>(tuple), count};
        }

        template <std::size_t Count>
        static constexpr auto operator()(index_t<Count> count) noexcept(
            noexcept(make_partial_closure(adaptor{}, count))
        ) {
            return make_partial_closure(adaptor{}, count);
        }
};

} // namespace _unspecified::take

inline constexpr _unspecified::take::adaptor take{};

} // namespace views

} // namespace yu::tuples

#endif
