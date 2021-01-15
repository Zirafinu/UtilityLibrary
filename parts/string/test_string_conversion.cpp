/*
 * test_crc.cpp
 *
 *  Created on: 08.01.2021
 *      Author: sebastian
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <from_string_conversion.hpp>
#include <string>
#include <to_string_conversion.hpp>

#include "doctest/doctest.h"

TEST_SUITE("from string conversion") {
    TEST_CASE("parseSign") {
        using utility::string_conversion::helpers::ParseSign;

        std::array<char, 1> sign{};

        sign[0] = '0';
        auto res = ParseSign<int>(sign.begin());
        CHECK(res.m_value == 1);
        CHECK(res.m_it == sign.begin());

        sign[0] = '-';
        res = ParseSign<int>(sign.begin());
        CHECK(res.m_value == -1);
        CHECK(res.m_it == sign.end());

        res = ParseSign<float>(sign.begin());
        CHECK(res.m_value == -1);
        CHECK(res.m_it == sign.end());

        res = ParseSign<unsigned>(sign.begin());
        CHECK(res.m_value == 1);
        CHECK(res.m_it == sign.begin());

        sign[0] = '+';
        res = ParseSign<int>(sign.begin());
        CHECK(res.m_value == 1);
        CHECK(res.m_it == sign.end());

        res = ParseSign<unsigned>(sign.begin());
        CHECK(res.m_value == 1);
        CHECK(res.m_it == sign.end());
    }

    template <typename T>
    void CheckParseBase() {
        std::array<char, 2> base{'0', '1'};

        using utility::string_conversion::helpers::BINARY_BASE;
        using utility::string_conversion::helpers::DECIMAL_BASE;
        using utility::string_conversion::helpers::HEXDEC_BASE;
        using utility::string_conversion::helpers::ParseBase;

        auto res = ParseBase<T>(base.begin(), base.end());
        CHECK(res.m_value == DECIMAL_BASE);
        CHECK(res.m_it == base.begin());

        base[1] = 'd';
        res = ParseBase<T>(base.begin(), base.end());
        CHECK(res.m_value == DECIMAL_BASE);
        CHECK(res.m_it == base.end());

        base[1] = 'x';
        res = ParseBase<T>(base.begin(), base.end());
        CHECK(res.m_value == HEXDEC_BASE);
        CHECK(res.m_it == base.end());

        base[1] = 'b';
        res = ParseBase<T>(base.begin(), base.end());
        CHECK(res.m_value == BINARY_BASE);
        CHECK(res.m_it == base.end());

        std::array<char, 1> base2{'0'};
        res = ParseBase<T>(base2.begin(), base2.end());
        CHECK(res.m_value == DECIMAL_BASE);
        CHECK(res.m_it == base2.begin());

        res = ParseBase<T>(base2.begin(), base2.begin());
        CHECK(res.m_value == DECIMAL_BASE);
        CHECK(res.m_it == base2.begin());
    }
    TEST_CASE("parseSign") {
        CheckParseBase<int>();
        CheckParseBase<unsigned>();
        CheckParseBase<float>();
    }

    TEST_CASE("parseDigit") {
        using utility::string_conversion::helpers::ParseDigit;
        CHECK(ParseDigit('0') == 0);
        CHECK(ParseDigit('9') == 9);
        CHECK(ParseDigit('a') == 10);
        CHECK(ParseDigit('z') == 35);
        CHECK(ParseDigit('A') == 10);
        CHECK(ParseDigit('\n') > 35);
    }

    TEST_CASE("parseInteger") {
        int result{};
        unsigned u_result{};

        using utility::string_conversion::FromString;
        using utility::string_conversion::IntFromString;

        const std::string txt{"123456789"};
        auto parsed_to = IntFromString<int>(txt.begin(), txt.end(), &result, 0);
        CHECK(parsed_to == txt.end());
        CHECK(result == 123456789);

        parsed_to = IntFromString<unsigned, true>(txt.begin(), txt.end(), &u_result, 0);
        CHECK(parsed_to == txt.end());
        CHECK(u_result == 123456789);

        const std::string xtxt{"-0x123456789"};
        parsed_to = IntFromString<int, true>(xtxt.begin(), xtxt.end(), &result, 0);
        CHECK(parsed_to == (xtxt.end() - 1));
        CHECK(result == -0x12345678);

        parsed_to = IntFromString<unsigned, true>(xtxt.begin(), xtxt.end(), &u_result, 0);
        CHECK(&*parsed_to == &*xtxt.begin());
        CHECK(u_result == 0);

        const std::string xtxt2{"0x123456789Abg"};
        parsed_to = IntFromString<int>(xtxt2.begin(), xtxt2.end(), &result, 0);
        CHECK(&*parsed_to == &*(xtxt2.end() - 1));
        CHECK(result == 0x456789ab);

        parsed_to = IntFromString<unsigned>(xtxt2.begin(), xtxt2.end(), &u_result, 0);
        CHECK(parsed_to == (xtxt2.end() - 1));
        CHECK(u_result == 0x456789ab);

        const std::string btxt{"+0b123456789"};
        parsed_to = IntFromString<int>(btxt.begin(), btxt.end(), &result, 0);
        CHECK(parsed_to == (btxt.begin() + 4));
        CHECK(result == 1);

        parsed_to = IntFromString<unsigned>(btxt.begin(), btxt.end(),
                                            gsl::strict_not_null<unsigned*>{&u_result}, 0);
        CHECK(parsed_to == (btxt.begin() + 4));
        CHECK(u_result == 1);

        std::string etxt{};
        parsed_to = FromString<int>(etxt.begin(), etxt.end(), &result);
        CHECK(parsed_to == etxt.end());
        CHECK(result == 1);
        parsed_to = FromString<unsigned>(etxt.begin(), etxt.end(),
                                         gsl::strict_not_null<unsigned*>{&u_result});
        CHECK(parsed_to == etxt.end());
        CHECK(u_result == 1);
    }

    TEST_CASE("pares float") {
        double result{};
        using utility::string_conversion::FloatFromString;

        const std::string txt{"123456789"};
        auto parsed_to = FloatFromString<double>(txt.begin(), txt.end(), &result);
        CHECK(parsed_to == txt.end());
        CHECK(result == 123456789);

        const std::string xtxt{"-0x123456789"};
        parsed_to = FloatFromString<double>(xtxt.begin(), xtxt.end(), &result);
        CHECK(parsed_to == xtxt.end());
        CHECK(result == -4886718345.0);  // NOLINT

        const std::string ftxt{"0.75"};
        parsed_to = FloatFromString<double>(ftxt.begin(), ftxt.end(), &result);
        CHECK(parsed_to == ftxt.end());
        CHECK(result == 0.75);  // NOLINT

        const std::string fxtxt{"0x0.0000000075apa"};
        parsed_to = FloatFromString<double>(fxtxt.begin(), fxtxt.end(), &result);
        CHECK(parsed_to == fxtxt.end());
        CHECK(result == 117.625);  // NOLINT

        const std::string fdmtxt{"100e-2"};
        parsed_to = FloatFromString<double>(fdmtxt.begin(), fdmtxt.end(), &result);
        CHECK(parsed_to == fdmtxt.end());
        CHECK(result == 1);

        const std::string fdptxt{"100p+2"};
        parsed_to = FloatFromString<double>(fdptxt.begin(), fdptxt.end(), &result);
        CHECK(parsed_to == fdptxt.end());
        CHECK(result == 10000);
    }
}

TEST_SUITE("to string conversion") {
    using utility::string_conversion::ToString;
    TEST_CASE("to empty string") {
        std::string txt{};
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
        auto used = ToString(txt.begin(), txt.end(), 1, 10);
        CHECK(used > 0);
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
        used = ToString(txt.begin(), txt.end(), 2.0F, 10);
        CHECK(used > 0);
    }

    TEST_CASE("to matching string") {
        std::string txt = "x";
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
        auto used = ToString(txt.begin(), txt.end(), 1, 10);
        CHECK(used == 1);
        CHECK(txt == std::string("1"));
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
        used = ToString(txt.begin(), txt.end(), 1.0F, 10);
        CHECK(used == 1);
        CHECK(txt == std::string("1"));
    }

    TEST_CASE("long int to matching string") {
        std::string txt{};
        txt.resize(8);  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
        auto used = ToString(txt.begin(), txt.end(), 12345678, 10);
        CHECK(used == txt.size());
        CHECK(txt == std::string("12345678"));
        txt.resize(100);  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
        used = ToString(txt.begin(), txt.end(), 2.3456789F, 10);
        CHECK(used == 23);
        txt.resize(used);
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
        CHECK(txt == std::string("2.345678806304931640625"));
        txt.resize(9);  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
        used = ToString(txt.begin(), txt.end(), 0.3456789F, 10);
        CHECK(used == txt.size());
        CHECK(txt == std::string("0.3456789"));
    }

    TEST_CASE("float exp notation") {
        std::string txt{};
        txt.resize(100);  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
        auto used = ToString(txt.begin(), txt.end(), 100.0e10F, 10);
        CHECK(used == 4);
        txt.resize(used);
        CHECK(txt == std::string("1e12"));
        txt.resize(8);  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
        used = ToString(txt.begin(), txt.end(), 999.99e10F, 10);
        CHECK(used == txt.size());
        txt.resize(used);
        CHECK(txt == std::string("9.999e12"));
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
        used = ToString(txt.begin(), txt.end(), 100.0e-14F, 10);
        CHECK(used == 5);
        txt.resize(used);
        CHECK(txt == std::string("1e-12"));
        txt.resize(9);  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
        used = ToString(txt.begin(), txt.end(), 999.99e-14F, 10);
        CHECK(used == 9);
        txt.resize(used);
        CHECK(txt == std::string("9.999e-12"));
    }

    TEST_CASE("sign test") {
        std::string txt{};
        txt.resize(100);  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
        auto used = ToString(txt.begin(), txt.end(), -125.0e10F, 10);
        CHECK(used == 8);
        txt.resize(8);  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
        CHECK(txt == std::string("-1.25e12"));
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
        used = ToString(txt.begin(), txt.end(), -1, 10);
        CHECK(used == 2);
        txt.resize(used);
        CHECK(txt == std::string("-1"));
    }

    TEST_CASE("base") {
        std::string txt{};
        txt.resize(100);  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
        auto used = ToString(txt.begin(), txt.end(), 0xF, 16);
        CHECK(used == 1);
        txt.resize(used);
        CHECK(txt == std::string("F"));
        txt.resize(100);  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
        used = ToString(txt.begin(), txt.end(), float(0x38D7C50000000LL), 16);
        CHECK(used == 9);
        txt.resize(used);
        CHECK(txt == std::string("3.8D7C5pC"));
        txt.resize(100);  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
        used = ToString(txt.begin(), txt.end(), float(-0x38D7C50000000LL), 16);
        CHECK(used == 10);
        txt.resize(used);
        CHECK(txt == std::string("-3.8D7C5pC"));
        txt.resize(100);  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
        used = ToString(txt.begin(), txt.end(), float(0x38D7C50000000LL), 2);
        CHECK(used == 30);
        txt.resize(used);
        CHECK(txt == std::string("1.110001101011111000101e110001"));
        txt.resize(100);  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
        used = ToString(txt.begin(), txt.end(), float(0x1000LL), 16);
        CHECK(used == 4);
        txt.resize(used);
        CHECK(txt == std::string("1000"));
    }
}
