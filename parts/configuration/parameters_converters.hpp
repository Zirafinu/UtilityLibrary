/*
 * converters.hpp
 *
 *  Created on: 12.01.2021
 *      Author: sebastian
 */

#ifndef PARAMETERS_CONVERTERS_HPP
#define PARAMETERS_CONVERTERS_HPP

#include <from_string_conversion.hpp>
#include <std_to_array_fwd.hpp>
#include <to_string_conversion.hpp>

#include "parameters.hpp"

namespace utility::parameters {

struct SErrorMessages {
    SErrorMessages() = delete;

    constexpr static auto ERROR_INVALID_CHARACTERS =
        general::std_fwd::to_array("Illegal characters given");
    constexpr static auto ERROR_TEXT_TOO_SHORT =
        general::std_fwd::to_array("To few characters given");
    constexpr static auto ERROR_TEXT_TOO_LONG =
        general::std_fwd::to_array("To many characters given");
    constexpr static auto ERROR_NUMBER_TOO_SMALL =
        general::std_fwd::to_array("To few digits given");
    constexpr static auto ERROR_NUMBER_TOO_LARGE =
        general::std_fwd::to_array("To many digits given");
};

template <typename ARRAY_TYPE, bool ALNUM_ONLY = true>
class CFixedArrayConverter : public CAValueConverter<ARRAY_TYPE> {
   public:
    [[nodiscard]] CAParameter::ErrorMessage Parse(const CAParameter::ValueString& string,
                                                  NotNull<ARRAY_TYPE> value) const override {
        if (string.size() < value->size()) {
            return {SErrorMessages::ERROR_TEXT_TOO_SHORT};
        }

        if (string.size() > value->size()) {
            return {SErrorMessages::ERROR_TEXT_TOO_LONG};
        }

        if constexpr (ALNUM_ONLY) {
            if (!std::all_of(string.begin(), string.end(),
                             [](char character) { return std::isalnum(character); })) {
                return {SErrorMessages::ERROR_INVALID_CHARACTERS};
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

template <typename NUMBER_TYPE>
class CNumberConverter : public CAValueConverter<NUMBER_TYPE> {
   public:
    [[nodiscard]] CAParameter::ErrorMessage Parse(const CAParameter::ValueString& string,
                                                  NotNull<NUMBER_TYPE> value) const override {
        using utility::string_conversion::FromString;
        if (string.empty()) {
            return {SErrorMessages::ERROR_NUMBER_TOO_SMALL};
        }

        if constexpr (std::is_integral<NUMBER_TYPE>::value &&
                      sizeof(NUMBER_TYPE) <= sizeof(std::size_t)) {
            using SubstituteNumberType =
                typename std::conditional<std::is_signed<NUMBER_TYPE>::value, std::ptrdiff_t,
                                          std::size_t>::type;

            SubstituteNumberType tmp_val{};
            auto it =
                FromString<SubstituteNumberType, true>(string.begin(), string.end(), &tmp_val);

            if (it != string.end()) {
                if (std::isdigit(*it) != 0) {
                    return {SErrorMessages::ERROR_NUMBER_TOO_LARGE};
                }
                return {SErrorMessages::ERROR_INVALID_CHARACTERS};
            }

            if (tmp_val != SubstituteNumberType(NUMBER_TYPE(tmp_val))) {
                return {SErrorMessages::ERROR_NUMBER_TOO_LARGE};
            }
            *value = NUMBER_TYPE(tmp_val);
            return CAParameter::NO_ERROR;
        } else {
            auto it = FromString<NUMBER_TYPE, true>(string.begin(), string.end(), value);
            if (it < string.end()) {
                if (std::isdigit(*it) != 0) {
                    return {SErrorMessages::ERROR_NUMBER_TOO_LARGE};
                }
                return {SErrorMessages::ERROR_INVALID_CHARACTERS};
            }
            return CAParameter::NO_ERROR;
        }
    }

    [[nodiscard]] CAParameter::UsedCharacters Stringify(CAParameter::OutputStringBuffer buffer,
                                                        const NUMBER_TYPE& value) const override {
        const NUMBER_TYPE default_base{10};
        using utility::string_conversion::ToString;
        return ToString<NUMBER_TYPE, true>(buffer.begin(), buffer.end(), value, default_base);
    }
};
}  // namespace utility::parameters

#endif /* CONVERTERS_HPP */
