/*
 * converters.hpp
 *
 *  Created on: 12.01.2021
 *      Author: sebastian
 */

#ifndef PARAMETERS_CONVERTERS_HPP
#define PARAMETERS_CONVERTERS_HPP

#include <from_string_conversion.hpp>
#include <to_string_conversion.hpp>

#include "parameters.hpp"


namespace utility::parameters {

template<typename ARRAY_TYPE>
class CFixedArrayConverter : public CAValueConverter<ARRAY_TYPE> {
public:
	constexpr static const char ERROR_TOOSHORT[] = "To few characters given";
	constexpr static const char ERROR_TOOLONG[] = "To many characters given";
	constexpr static const char ERROR_INVALID[] = "Illegal characters given";

	[[nodiscard]] CAParameter::ErrorMessage Parse(const CAParameter::ValueString& string,
			NotNull<ARRAY_TYPE> value) const override {
		if(string.size() < value->size()) {
			return CAParameter::ErrorMessage{&ERROR_TOOSHORT[0], std::size_t(sizeof(ERROR_TOOSHORT))};
			}
		if(string.size() > value->size()) {
			return CAParameter::ErrorMessage{&ERROR_TOOLONG[0], std::size_t(sizeof(ERROR_TOOLONG))};
			}
		if constexpr (true) {
			if(!std::all_of(string.begin(), string.end(), [](char character){return std::isalnum(character);})) {
				return CAParameter::ErrorMessage{&ERROR_INVALID[0], std::size_t(sizeof(ERROR_INVALID))};
			}
		}

		std::copy(string.begin(), string.end(), value->begin());
		return CAParameter::NO_ERROR;
	}

	[[nodiscard]] CAParameter::UsedCharacters Stringify(CAParameter::OutputStringBuffer buffer,
			const ARRAY_TYPE& value) const override {
		const auto copy_length = std::min(buffer.size(), value.size());
		std::copy_n(value.begin(), copy_length, buffer.begin());
		return value.size();
	}
};

template<typename NUMBER_TYPE>
class CNumberConverter : public CAValueConverter<NUMBER_TYPE> {
public:
	constexpr static const char ERROR_TOOSHORT[] = "To few digits given";
	constexpr static const char ERROR_TOOLONG[] = "To many digits given";
	constexpr static const char ERROR_INVALID[] = "Illegal characters given";



	[[nodiscard]] CAParameter::ErrorMessage Parse(const CAParameter::ValueString& string,
			NotNull<NUMBER_TYPE> value) const override {
		if(string.empty()){
			return CAParameter::ErrorMessage{&ERROR_TOOSHORT[0], std::size_t(sizeof(ERROR_TOOSHORT))};
		}

		if constexpr(std::is_integral<NUMBER_TYPE>::value && sizeof(NUMBER_TYPE) <= sizeof(std::size_t)) {
			using SubstituteNumberType = typename std::conditional<std::is_signed<NUMBER_TYPE>::value, std::ptrdiff_t, std::size_t>::type;

			SubstituteNumberType tmp_val{};
			auto it = utility::string_conversion::FromString<SubstituteNumberType, true>(string.begin(), string.end(), &tmp_val);
			if(it != string.end()) {
				if(std::isdigit(*it) != 0) {
					return CAParameter::ErrorMessage{&ERROR_TOOLONG[0], std::size_t(sizeof(ERROR_TOOLONG))};
				}
				return CAParameter::ErrorMessage{&ERROR_INVALID[0], std::size_t(sizeof(ERROR_INVALID))};
			}
			if(tmp_val != SubstituteNumberType(NUMBER_TYPE(tmp_val))) {
				return CAParameter::ErrorMessage{&ERROR_TOOLONG[0], std::size_t(sizeof(ERROR_TOOLONG))};
			}
			*value = NUMBER_TYPE(tmp_val);
			return CAParameter::NO_ERROR;
		} else {
			auto it = utility::string_conversion::FromString<NUMBER_TYPE, true>(string.begin(), string.end(), value);
			if(it < string.end()) {
				if(std::isdigit(*it) != 0) {
					return CAParameter::ErrorMessage{&ERROR_TOOLONG[0], std::size_t(sizeof(ERROR_TOOLONG))};
				}
				return CAParameter::ErrorMessage{&ERROR_INVALID[0], std::size_t(sizeof(ERROR_INVALID))};
			}
			return CAParameter::NO_ERROR;
		}
	}

	[[nodiscard]] CAParameter::UsedCharacters Stringify(CAParameter::OutputStringBuffer buffer,
			const NUMBER_TYPE& value) const override {
		const NUMBER_TYPE default_base{10};
		return utility::string_conversion::ToString<NUMBER_TYPE, true>(buffer.begin(), buffer.end(), value, default_base);
	}
};
} // namespace utility::parameters

#endif /* CONVERTERS_HPP */
