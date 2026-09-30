// yutool: include guard
#ifndef YU_TUPLES_VIEWS_DETAIL_DROP_VIEW_BASE_HPP_
#define YU_TUPLES_VIEWS_DETAIL_DROP_VIEW_BASE_HPP_

#include <yu/tuples/access/index.hpp>
#include <yu/tuples/concepts/view.hpp>
#include <yu/tuples/type_traits/element_type.hpp>
#include <yu/tuples/views/all.hpp>
#include <yu/tuples/views/view_interface.hpp>
#include <cstddef>
#include <utility>

namespace yu::tuples::_detail {

template <typename View, std::size_t Count>
class drop_view_base {
    private:
        View base_;

        static constexpr auto base_size_  = tuples::size<View>{};
        static constexpr auto drop_count_ = index<((Count < base_size_) ? Count : base_size_)>;

    public:
        static constexpr auto size = base_size_ - drop_count_;

        constexpr explicit drop_view_base(View view) :
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

} // namespace yu::tuples::_detail

#endif
