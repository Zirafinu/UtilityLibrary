/*
 * to_string_conversion.hpp
 *
 *  Created on: 13.01.2021
 *      Author: sebastian
 */

#ifndef TO_STRING_CONVERSION_HPP
#define TO_STRING_CONVERSION_HPP

#include <cctype>
#include <cmath>

#include <gsl/pointers>

#include "conversion_constants.hpp"

namespace utility::string_conversion {

namespace helpers{

template<typename T, bool MAKE_UNSIGNED>
struct ConversionTypeSelector;

template<typename T>
struct ConversionTypeSelector<T, false> {
	using type = T; //NOLINT(readability-identifier-naming)
};
template<typename T>
struct ConversionTypeSelector<T, true> {
	using type = typename std::make_unsigned<T>::type; //NOLINT(readability-identifier-naming)
};

template<typename T>
struct SConversionType : public ConversionTypeSelector<T, std::is_integral<T>::value && std::is_signed<T>::value>{};


[[nodiscard]] constexpr auto DigitToCharacter(uint_fast8_t number) noexcept -> uint_fast8_t {
	if(number > DECIMAL_BASE){
		return 'A' - DECIMAL_BASE + number;
	}
	return '0' + number;
}
} // namespace helpers

template<typename T>
using NotNull = gsl::not_null<T*>;

template<bool CHECK_OVERFLOW, typename T, typename IT_T = char*>
[[nodiscard]] constexpr auto IntToString(IT_T begin, IT_T end, T value, uint_fast8_t base) -> size_t {
	if constexpr(CHECK_OVERFLOW) {
		if(begin == end){
			return 1;
		}
	}

	auto current_it = begin;
	if constexpr (std::is_signed<T>::value) {
		if(value < 0) {
			*current_it = '-';
			++current_it;
			if constexpr(CHECK_OVERFLOW) {
				if(current_it == end) {
					return current_it - begin + 1;
				}
			}
			value *= -1;
		}
	}

	// this is required since int::min * -1 == int::min
	// and this is also used for the integer part of floats
	using CVT = typename helpers::SConversionType<T>::type;
	if constexpr(std::is_integral<T>::value && std::is_signed<T>::value) {
		auto number_length = IntToString<CHECK_OVERFLOW>(current_it, end, CVT(value), base);
		return number_length + current_it - begin;
	} else {
		const auto ipart_it = current_it;
		do {
			if constexpr(CHECK_OVERFLOW) {
				if(current_it == end) {
					std::reverse(ipart_it, current_it);
					return current_it - begin + 1;
				}
			}
			if constexpr (std::is_integral<T>::value) {
				*current_it = helpers::DigitToCharacter(value % base);
			} else {
				*current_it = helpers::DigitToCharacter(std::fmod(value, base));
			}
			value /= base;
			++current_it;
		} while(std::floor(value) > 0);

		std::reverse(ipart_it, current_it);
		return current_it - begin;
	}
}

template<bool CHECK_OVERFLOW, typename T, typename IT_T = char*>
[[nodiscard]] constexpr auto FloatToString(IT_T begin, IT_T end, T value, uint_fast8_t base) -> size_t{
	if constexpr(CHECK_OVERFLOW) {
		if(begin == end) {
			return 1;
		}
	}

	auto current_it = begin;

	int exponent{};
	std::frexp(value, &exponent);
	const int exp_limit = 20;
	if(exponent > exp_limit || exponent < -exp_limit) {
		// 2^20 ~ 1 million
		int new_base_exp = std::floor(T(exponent) / std::log2(T(base)));
		const int nb_exp_limit = 9;
		if(new_base_exp >= nb_exp_limit || new_base_exp <= -nb_exp_limit) {
			const auto base_correction = std::pow(T(base), new_base_exp);
			value /= base_correction;
			if(std::abs(value) < 1) {
				new_base_exp -= 1;
				value *= base;
			}
			*current_it = base <= helpers::DECIMAL_BASE ? 'e' : 'p';
			++current_it;
			if constexpr(CHECK_OVERFLOW) {
				if(current_it == end) {
					return current_it - begin + 1;
				}
			}
			current_it += IntToString<CHECK_OVERFLOW>(current_it, end, new_base_exp, base);
			if constexpr(CHECK_OVERFLOW) {
				if(current_it == end) {
					return current_it - begin + 1;
				}
			}
		}
	}
	const auto exp_rotate_end = current_it;

	T integer_part{};
	T fract = std::abs(std::modf(value, &integer_part));
	current_it += IntToString<CHECK_OVERFLOW>(current_it, end, integer_part, base);

	if(fract != 0 && !std::isnan(fract)) {
		if constexpr(CHECK_OVERFLOW) {
			if(current_it == end) {
				std::rotate(begin, exp_rotate_end, current_it);
				return current_it - begin + 1;
			}
		}

		*current_it = '.';
		++current_it;

		while(fract != 0) {
			if constexpr(CHECK_OVERFLOW) {
				if(current_it == end) {
					break;
				}
			}
			fract = fract * base;
			uint_fast8_t current{};
			if(current_it == end - 1) {
				current = uint_fast8_t(std::round(fract));
				if(current == base) {
					current -= 1;
				}
			} else {
				current = uint_fast8_t(fract);
				fract = fract - current;
			}
			*current_it = helpers::DigitToCharacter(current);
			++current_it;
		}
	}
	std::rotate(begin, exp_rotate_end, current_it);
	return current_it - begin;
}

template<typename T, bool CHECK_OVERFLOW = true, typename IT_T = char*>
[[nodiscard]] constexpr auto ToString(IT_T begin, IT_T end, const T& value, uint_fast8_t base) -> size_t {

	if constexpr (std::is_floating_point_v<T>) {
		return FloatToString<CHECK_OVERFLOW, T>(begin, end, value, base);
	} else if constexpr (std::is_integral_v<T>) {
		return IntToString<CHECK_OVERFLOW, T>(begin, end, value, base);
	} else {
		static_assert(std::is_floating_point_v<T> || std::is_integral_v<T>);
	}
	return 0;
}

}//namespace utility::string_conversion

#endif /* TO_STRING_CONVERSION_HPP */
