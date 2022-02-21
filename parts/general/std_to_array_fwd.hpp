/*
 * std_to_array_fwd.hpp
 *
 *  Created on: 15.01.2021
 *      Author: sebastian
 */

#ifndef STD_TO_ARRAY_FWD_HPP
#define STD_TO_ARRAY_FWD_HPP

#include <array>

namespace utility::general::std_fwd {

namespace detail {

template <class T, std::size_t N, std::size_t... I>
constexpr std::array<std::remove_cv_t<T>, N> ToArrayImpl(T (&a)[N],
                                                         std::index_sequence<I...> /*unused*/) {
    return {{a[I]...}};
}

}  // namespace detail

template <class T, std::size_t N>
constexpr std::array<std::remove_cv_t<T>, N> to_array(T (&a)[N]) {
    return detail::ToArrayImpl(a, std::make_index_sequence<N>{});
}

}  // namespace utility::general::std_fwd

#endif /* STD_TO_ARRAY_FWD_HPP */
