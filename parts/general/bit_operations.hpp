/*
 * bit_operations.hpp
 *
 *  Created on: 15.02.2021
 *      Author: sebastian
 */

#ifndef BIT_OPERATIONS_HPP
#define BIT_OPERATIONS_HPP

#include <climits>
#include <cstdint>
#include <type_traits>

namespace utility::general::bit_ops {

template <typename T>
[[nodiscard]] constexpr auto RevertBits(const T &value) noexcept -> T {
    static_assert(std::is_integral<T>::value, "Type has to be integral");
    static_assert(std::is_unsigned<T>::value, "Type has to be unsigned");
    T result{value};
    T mask = ~T(0);
    std::size_t bits = CHAR_BIT * sizeof(T) / 2;
    while (bits > 0) {
        mask ^= T(mask << bits);
        result = T(T(result >> bits) & mask) | T(T(result & mask) << bits);
        bits /= 2;
    }
    return result;
}

}  // namespace utility::general::bit_ops

#endif /* BIT_OPERATIONS_HPP */
