/*
 * CCrc8.hpp
 *
 *  Created on: 11.12.2020
 *      Author: sebastian
 */

#ifndef STRING_CONVERSION_HPP
#define STRING_CONVERSION_HPP

#include <cctype>
#include <cmath>
#include <gsl/pointers>

#include "conversion_constants.hpp"

namespace utility::string_conversion {

namespace helpers {
template <typename T>
using NotNull = gsl::not_null<T *>;

template <typename T, typename IT_T>
struct SParsingResult {
    T m_value;
    IT_T m_it;
};

template <typename T, typename IT_T>
[[nodiscard]] constexpr auto ParseSign(IT_T cur_char_it) -> SParsingResult<int_fast8_t, IT_T> {
    auto c_char = *cur_char_it;

    if constexpr (std::is_signed_v<T>) {
        if (c_char == '-') {
            return {-1, ++cur_char_it};
        }
    }

    if (c_char == '+') {
        return {1, ++cur_char_it};
    }
    return {1, cur_char_it};
}

template <typename T, typename IT_T>
[[nodiscard]] constexpr auto ParseBase(IT_T cur_char_it, const IT_T &end_char_it)
    -> SParsingResult<T, IT_T> {
    if (*cur_char_it == '0' && std::distance(cur_char_it, end_char_it) >= 2) {
        switch (char(std::tolower(cur_char_it[1]))) {
            case 'b':
                return {BINARY_BASE, cur_char_it + 2};
            case 'd':
                return {DECIMAL_BASE, cur_char_it + 2};
            case 'x':
                return {HEXDEC_BASE, cur_char_it + 2};
            default:
                break;
        }
    }
    return {DECIMAL_BASE, cur_char_it};
}

template <typename CHAR_TYPE>
[[nodiscard]] constexpr auto ParseDigit(CHAR_TYPE current) noexcept -> uint_fast8_t {
    const uint8_t value = current - '0';
    if (value >= DECIMAL_BASE) {
        return (char(std::tolower(current)) - 'a' + DECIMAL_BASE);
    }
    return value;
}
}  // namespace helpers

template <typename T, bool CHECK_OVERFLOWS = false, typename IT_T>
[[nodiscard]] constexpr auto IntFromString(IT_T cur_char_it, IT_T end_char_it,
                                           helpers::NotNull<T> result, T base = 0) -> IT_T {
    const auto res = helpers::ParseSign<T>(cur_char_it);
    const auto sign = res.m_value;
    cur_char_it = res.m_it;
    if (base == 0) {
        const auto res = helpers::ParseBase<T>(cur_char_it, end_char_it);
        base = res.m_value;
        cur_char_it = res.m_it;
    }

    T tmp = 0;
    {
        for (; cur_char_it < end_char_it; ++cur_char_it) {
            auto current = helpers::ParseDigit(*cur_char_it);
            if (current >= base) {
                break;
            }
            if constexpr (CHECK_OVERFLOWS) {
                const T tmp_limit = std::numeric_limits<T>::max() / base;
                if (tmp > tmp_limit) {
                    break;
                }
            }
            tmp = (tmp * base) + current;
        }
    }

    *result = tmp * sign;
    return cur_char_it;
}

template <typename T, typename IT_T>
[[nodiscard]] constexpr auto FloatFromString(IT_T cur_char_it, IT_T end_char_it,
                                             helpers::NotNull<T> result) -> IT_T {
    static_assert(std::is_floating_point_v<T>);

    const auto sign_res = helpers::ParseSign<T>(cur_char_it);
    const auto sign = sign_res.m_value;
    cur_char_it = sign_res.m_it;

    const auto base_res = helpers::ParseBase<T>(cur_char_it, end_char_it);
    const T base = base_res.m_value;
    cur_char_it = base_res.m_it;

    T integer_part{0};
    cur_char_it =
        IntFromString<T>(cur_char_it, end_char_it, helpers::NotNull<T>{&integer_part}, base);
    if (cur_char_it != end_char_it && *cur_char_it == '.') {
        ++cur_char_it;
        T fractional_part{0};
        const auto start_it = cur_char_it;
        cur_char_it =
            IntFromString<T>(cur_char_it, end_char_it, helpers::NotNull<T>{&fractional_part}, base);
        integer_part += fractional_part / std::pow(base, T(cur_char_it - start_it));
    }

    T exponent_part{1};
    if ((cur_char_it != end_char_it) &&
        ((char(std::tolower(*cur_char_it)) == 'e' && base <= helpers::DECIMAL_BASE) ||
         (char(std::tolower(*cur_char_it)) == 'p'))) {
        ++cur_char_it;
        int32_t exp{};
        cur_char_it = IntFromString<int32_t, true>(cur_char_it, end_char_it,
                                                   helpers::NotNull<int32_t>{&exp}, base);
        exponent_part = std::pow(T(base), T(exp));
    }
    *result = integer_part * sign * exponent_part;
    return cur_char_it;
}

template <typename T, bool CHECK_OVERFLOW = false, typename IT_T = char *>
[[nodiscard]] constexpr auto FromString(IT_T begin, IT_T end, helpers::NotNull<T> result) -> IT_T {
    if (begin == end) {
        return begin;
    }

    if constexpr (std::is_floating_point_v<T>) {
        return FloatFromString<T>(begin, end, result);
    } else if constexpr (std::is_integral_v<T>) {
        return IntFromString<T, CHECK_OVERFLOW>(begin, end, result);
    } else {
        static_assert(std::is_floating_point_v<T> || std::is_integral_v<T>);
    }
    return begin;
}

}  // namespace utility::string_conversion

#endif /* STRING_CONVERSION_HPP */
