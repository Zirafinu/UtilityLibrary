/*
 * test_converters.cpp
 *
 *  Created on: 15.01.2021
 *      Author: sebastian
 */

#include <numeric>

//#define DOCTEST_CONFIG_
#include "doctest/doctest.h"

#include "parameters.hpp"
#include "parameters_converters.hpp"

constexpr std::size_t GC_SERIAL_NUMBER_LENGTH = 8;
using SerialType = std::array<char, GC_SERIAL_NUMBER_LENGTH>;
using CAParameter = utility::parameters::CAParameter;
using SerialValueConverter = utility::parameters::CFixedArrayConverter<SerialType>;

template <typename T>
std::string ToString(const T& array) {
    return std::accumulate(std::begin(array), std::end(array), std::string{});
}

using utility::parameters::SErrorMessages;

TEST_SUITE("serial converter") {
    using SerialConverter = utility::parameters::CFixedArrayConverter<SerialType>;
    TEST_CASE("serial exact match converter") {
        SerialValueConverter converter{};

        std::string txt = "12345678";
        SerialType value{};
        CHECK(ToString(converter.Parse(txt, &value)) == ToString(CAParameter::NO_ERROR));
        CHECK(ToString(value) == txt);
    }

    TEST_CASE("serial too short") {
        SerialValueConverter converter{};

        SerialType value{0};
        std::string serial_expected = ToString(value);
        std::string txt = "1234567";
        CHECK(ToString(converter.Parse(txt, &value)) ==
              ToString(SErrorMessages::ERROR_TEXT_TOO_SHORT));
        std::string serial = ToString(value);
        CHECK(serial == serial_expected);
    }

    TEST_CASE("serial too long") {
        SerialValueConverter converter{};

        SerialType value{0};
        std::string serial_expected = ToString(value);
        std::string txt = "123456789";
        CHECK(ToString(converter.Parse(txt, &value)) ==
              ToString(SErrorMessages::ERROR_TEXT_TOO_LONG));
        std::string serial = ToString(value);
        CHECK(serial == serial_expected);
    }

    TEST_CASE("serial bad characters") {
        SerialValueConverter converter{};

        SerialType value{0};
        std::string serial_expected = ToString(value);
        std::string txt = "1234567!";
        CHECK(ToString(converter.Parse(txt, &value)) ==
              ToString(SErrorMessages::ERROR_INVALID_CHARACTERS));
        std::string serial = ToString(value);
        CHECK(serial == serial_expected);

        txt = "1234567,";
        CHECK(ToString(converter.Parse(txt, &value)) ==
              ToString(SErrorMessages::ERROR_INVALID_CHARACTERS));
        txt = "1234567.";
        CHECK(ToString(converter.Parse(txt, &value)) ==
              ToString(SErrorMessages::ERROR_INVALID_CHARACTERS));
        txt = "1234567\n";
        CHECK(ToString(converter.Parse(txt, &value)) ==
              ToString(SErrorMessages::ERROR_INVALID_CHARACTERS));
        txt = "1234567)";
        CHECK(ToString(converter.Parse(txt, &value)) ==
              ToString(SErrorMessages::ERROR_INVALID_CHARACTERS));
    }

    TEST_CASE("serial to string") {
        SerialValueConverter converter{};
        SerialType value{'a', 0, '0', 'b', 'c', 'd', 'e', 'f'};
        std::string buffer{};
        buffer.resize(200);  // NOLINT
        gsl::span<char, gsl::dynamic_extent> bufferspan{&*buffer.begin(), &*buffer.end()};

        auto count = converter.Stringify(bufferspan, value);
        CHECK(count == 8);
        buffer.resize(count + 1);
        CHECK(buffer[count + 1] == '\0');
        buffer.resize(count);
        CHECK(buffer == ToString(value));
    }

    TEST_CASE("serial to string buffer too short") {
        SerialValueConverter converter{};
        SerialType value{'a', '1', '0', 'b', 'c', 'd', 'e', 'f'};
        std::string buffer{};
        buffer.resize(7);  // NOLINT
        gsl::span<char, gsl::dynamic_extent> bufferspan{&*buffer.begin(), &*buffer.end()};

        auto count = converter.Stringify(bufferspan, value);
        CHECK(count == 8);
        CHECK(buffer == std::accumulate(std::begin(value), std::end(value) - 1, std::string{}));
    }
}

TEST_SUITE("number converter") {
    using utility::parameters::CAParameter;
    using utility::parameters::CNumberConverter;
    TEST_CASE("empty string") {
        CNumberConverter<int> conv{};
        std::string txt{};
        int i = -1;
        auto err = conv.Parse(txt, &i);
        CHECK(i == -1);
        CHECK(ToString(err) == ToString(SErrorMessages::ERROR_NUMBER_TOO_SMALL));
    }

    TEST_CASE("valid string") {
        CNumberConverter<int> conv{};
        std::string txt = "1";
        int i = -1;
        auto err = conv.Parse(txt, &i);
        CHECK(i == 1);
        CHECK(ToString(err) == ToString(CAParameter::NO_ERROR));

        CNumberConverter<unsigned> uconv{};
        unsigned u = 0;
        err = uconv.Parse(txt, &u);
        CHECK(u == 1);
        CHECK(ToString(err) == ToString(CAParameter::NO_ERROR));
    }

    TEST_CASE("overflow target") {
        CNumberConverter<char> conv{};
        std::string txt = "128";
        char i = -1;
        auto err = conv.Parse(txt, &i);
        CHECK(i == -1);
        CHECK(ToString(err) == ToString(SErrorMessages::ERROR_NUMBER_TOO_LARGE));
    }

    TEST_CASE("long long target") {
        CNumberConverter<int64_t> conv{};
        std::string txt = "0x123456789ABCDEF";
        int64_t i = -1;
        auto err = conv.Parse(txt, &i);
        CHECK(i == 0x123456789ABCDEFLL);
        CHECK(ToString(err) == ToString(CAParameter::NO_ERROR));
    }

    TEST_CASE("int to string") {
        CNumberConverter<int> conv{};
        std::string txt{};
        int i = -1;
        auto used = conv.Stringify(txt, i);
        CHECK(used > 0);
        txt.resize(2);
        used = conv.Stringify(txt, i);
        CHECK(used == 2);
        CHECK(txt == std::string("-1"));
    }

    TEST_CASE("float target") {
        CNumberConverter<float> conv{};
        std::string txt = "-3.75e0";
        float i = -1;
        auto err = conv.Parse(txt, &i);
        CHECK(i == -3.75F);  // NOLINT
        CHECK(ToString(err) == ToString(CAParameter::NO_ERROR));
    }
}
