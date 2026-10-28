// yutool: include guard
#ifndef YU_TUPLES_ACCESS_GET_HPP_
#define YU_TUPLES_ACCESS_GET_HPP_

#include "index.hpp"
#include <yu/config/stateless_call_operator.hpp>
#include <cstddef>
#include <utility>

namespace yu::tuples {

namespace fn {

// Poison Pill
template <std::size_t>
void get() = delete;

struct get_fn {
        /** 固定長配列の左辺値参照であるTupleから，指定位置の要素を取得する．

            @param t 固定長配列の左辺値参照

            @return 指定位置の要素
        */
        template <typename T, std::size_t N, std::size_t Idx>
        requires (Idx < N)
        constexpr auto YU_STATELESS_CALL_OPERATOR(T (&t)[N], index_t<Idx>) noexcept -> T& {
            return t[Idx];
        }

        /** 固定長配列の右辺値参照であるTupleから，指定位置の要素を取得する．

            @param t 固定長配列の右辺値参照

            @return 指定位置の要素
        */
        template <typename T, std::size_t N, std::size_t Idx>
        requires (Idx < N)
        constexpr auto YU_STATELESS_CALL_OPERATOR(T (&&t)[N], index_t<Idx>) noexcept -> T&& {
            return std::move(t)[Idx];
        }

        /** Tupleの指定位置の要素を，メンバ関数テンプレート`get`を用いて取得する．

            @param t メンバ関数テンプレート`get`をサポートするTuple．

            @return 指定位置の要素

            @note 呼び出しを`get(t, index<Idx>)`とするとき，`t.template get<Idx>()`と等価．
        */
        template <typename T, std::size_t Idx>
        requires requires(T&& t) { std::forward<T>(t).template get<Idx>(); }
        constexpr auto YU_STATELESS_CALL_OPERATOR(T&& t, index_t<Idx>) noexcept(
            noexcept(std::forward<T>(t).template get<Idx>())
        ) -> decltype(std::forward<T>(t).template get<Idx>()) {
            return std::forward<T>(t).template get<Idx>();
        }

        /** Tupleの指定位置の要素を，非メンバ関数テンプレート`get`を用いて取得する．

            @param t ADLで探索された関数テンプレート`get`が有効なTuple．

            @return 指定位置の要素

            @note 呼び出しを`get(t, index<Idx>)`とするとき，`get<Idx>(t)`と等価．
        */
        template <typename T, std::size_t Idx>
        constexpr auto YU_STATELESS_CALL_OPERATOR(T&& t, index_t<Idx>) noexcept(noexcept(get<Idx>(std::forward<T>(t))))
            -> decltype(get<Idx>(std::forward<T>(t))) {
            return get<Idx>(std::forward<T>(t));
        }
};

} // namespace fn

inline namespace _cpo {

/// @functor
inline constexpr fn::get_fn get;

} // namespace _cpo

} // namespace yu::tuples

#endif
