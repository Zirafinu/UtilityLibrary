/*
 * test_crc.cpp
 *
 *  Created on: 08.01.2021
 *      Author: sebastian
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include <string>

#include <from_string_conversion.hpp>
#include <to_string_conversion.hpp>

TEST_SUITE("from string conversion") {
	TEST_CASE("parseSign") {
		using utility::string_conversion::helpers::ParseSign;

		char sign{};
		char* signptr{};

		sign = '0';
		signptr = &sign;
		CHECK(ParseSign<int, char*>(&signptr) == 1);
		CHECK(signptr == &sign);

		sign = '-';
		signptr = &sign;
		CHECK(ParseSign<int, char*>(&signptr) == -1);
		CHECK(signptr == (&sign + 1));

		signptr = &sign;
		CHECK(ParseSign<float, char*>(&signptr) == -1);
		CHECK(signptr == (&sign + 1));

		signptr = &sign;
		CHECK(ParseSign<unsigned, char*>(&signptr) == 1);
		CHECK(signptr == &sign);

		sign = '+';
		signptr = &sign;
		CHECK(ParseSign<int, char*>(&signptr) == 1);
		CHECK(signptr == (&sign + 1));
		signptr = &sign;
		CHECK(ParseSign<unsigned, char*>(&signptr) == 1);
		CHECK(signptr == (&sign + 1));
	}

	template<typename T>
	void Check_parseBase(){
		std::array<char, 2> base {'0', '1'};
		auto base_ptr = base.begin();

		using utility::string_conversion::helpers::ParseBase;
		using utility::string_conversion::helpers::DECIMAL_BASE;
		using utility::string_conversion::helpers::BINARY_BASE;
		using utility::string_conversion::helpers::HEXDEC_BASE;

		CHECK(ParseBase<T, char*>(&base_ptr, base.end()) == DECIMAL_BASE);
		CHECK(base_ptr == base.begin());

		base[1] = 'd';
		base_ptr = base.begin();
		CHECK(ParseBase<T, char*>(&base_ptr, base.end()) == DECIMAL_BASE);
		CHECK(base_ptr == base.end());

		base[1] = 'x';
		base_ptr = base.begin();
		CHECK(ParseBase<T, char*>(&base_ptr, base.end()) == HEXDEC_BASE);
		CHECK(base_ptr == base.end());

		base[1] = 'b';
		base_ptr = base.begin();
		CHECK(ParseBase<T, char*>(&base_ptr, base.end()) == BINARY_BASE);
		CHECK(base_ptr == base.end());

		std::array<char, 1> base2 {'0'};
		auto base2_ptr = base2.begin();
		CHECK(ParseBase<T, char*>(&base2_ptr, base2.end()) == DECIMAL_BASE);
		CHECK(base2_ptr == base2.begin());

		base2_ptr = base2.begin();
		CHECK(ParseBase<T, char*>(&base2_ptr, base2.begin()) == DECIMAL_BASE);
		CHECK(base2_ptr == base2.begin());
	}
	TEST_CASE("parseSign") {
		Check_parseBase<int>();
		Check_parseBase<unsigned>();
		Check_parseBase<float>();
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
		int result;
		unsigned u_result;

		using utility::string_conversion::IntFromString;
		using utility::string_conversion::FromString;

		const std::string txt {"123456789"};
		auto parsed_to = IntFromString<int>(txt.begin(), txt.end(), &result, 0);
		CHECK(parsed_to == txt.end());
		CHECK(result == 123456789);

		parsed_to = IntFromString<unsigned, true>(txt.begin(), txt.end(), &u_result, 0);
		CHECK(parsed_to == txt.end());
		CHECK(u_result == 123456789);

		const std::string xtxt {"-0x123456789"};
		parsed_to = IntFromString<int, true>(xtxt.begin(), xtxt.end(), &result, 0);
		CHECK(parsed_to == (xtxt.end() - 1));
		CHECK(result == -0x12345678);

		parsed_to = IntFromString<unsigned, true>(xtxt.begin(), xtxt.end(), &u_result, 0);
		CHECK(&*parsed_to == &*xtxt.begin());
		CHECK(u_result == 0);

		const std::string xtxt2 {"0x123456789Abg"};
		parsed_to = IntFromString<int>(xtxt2.begin(), xtxt2.end(), &result, 0);
		CHECK(&*parsed_to == &*(xtxt2.end() - 1));
		CHECK(result == 0x456789ab);

		parsed_to = IntFromString<unsigned>(xtxt2.begin(), xtxt2.end(), &u_result, 0);
		CHECK(parsed_to == (xtxt2.end() - 1));
		CHECK(u_result == 0x456789ab);


		const std::string btxt {"+0b123456789"};
		parsed_to = IntFromString<int>(btxt.begin(), btxt.end(), &result, 0);
		CHECK(parsed_to == (btxt.begin() + 4));
		CHECK(result == 1);

		parsed_to = IntFromString<unsigned>(btxt.begin(), btxt.end(), gsl::strict_not_null<unsigned*>{&u_result}, 0);
		CHECK(parsed_to == (btxt.begin() + 4));
		CHECK(u_result == 1);

		std::string etxt = "";
		parsed_to = FromString<int>(etxt.begin(), etxt.end(), &result);
		CHECK(parsed_to == etxt.end());
		CHECK(result == 1);
		parsed_to = FromString<unsigned>(etxt.begin(), etxt.end(), gsl::strict_not_null<unsigned*>{&u_result});
		CHECK(parsed_to == etxt.end());
		CHECK(u_result == 1);
	}

	TEST_CASE("pares float") {
		double result;
		using utility::string_conversion::FloatFromString;

		const std::string txt {"123456789"};
		auto parsed_to = FloatFromString<double>(txt.begin(), txt.end(), &result);
		CHECK(parsed_to == txt.end());
		CHECK(result == 123456789);

		const std::string xtxt {"-0x123456789"};
		parsed_to = FloatFromString<double>(xtxt.begin(), xtxt.end(), &result);
		CHECK(parsed_to == xtxt.end());
		CHECK(result == -4886718345.0);

		const std::string ftxt {"0.75"};
		parsed_to = FloatFromString<double>(ftxt.begin(), ftxt.end(), &result);
		CHECK(parsed_to == ftxt.end());
		CHECK(result == 0.75);

		const std::string fxtxt {"0x0.0000000075apa"};
		parsed_to = FloatFromString<double>(fxtxt.begin(), fxtxt.end(), &result);
		CHECK(parsed_to == fxtxt.end());
		CHECK(result == 117.625);

		const std::string fdmtxt {"100e-2"};
		parsed_to = FloatFromString<double>(fdmtxt.begin(), fdmtxt.end(), &result);
		CHECK(parsed_to == fdmtxt.end());
		CHECK(result == 1);

		const std::string fdptxt {"100p+2"};
		parsed_to = FloatFromString<double>(fdptxt.begin(), fdptxt.end(), &result);
		CHECK(parsed_to == fdptxt.end());
		CHECK(result == 10000);
	}
}

TEST_SUITE("to string conversion") {
	using utility::string_conversion::ToString;
	TEST_CASE("to empty string") {
		std::string txt = "";
		auto used = ToString(txt.begin(), txt.end(), 1, 10);
		CHECK(used > 0);
		used = ToString(txt.begin(), txt.end(), 2.0f, 10);
		CHECK(used > 0);
	}

	TEST_CASE("to matching string") {
		std::string txt = "x";
		auto used = ToString(txt.begin(), txt.end(), 1, 10);
		CHECK(used == 1);
		CHECK(txt == std::string("1"));
		used = ToString(txt.begin(), txt.end(), 1.0f, 10);
		CHECK(used == 1);
		CHECK(txt == std::string("1"));
	}

	TEST_CASE("long int to matching string") {
		std::string txt{};
		txt.resize(8);
		auto used = ToString(txt.begin(), txt.end(), 12345678, 10);
		CHECK(used == 8);
		CHECK(txt == std::string("12345678"));
		txt.resize(100);
		used = ToString(txt.begin(), txt.end(), 2.3456789f, 10);
		CHECK(used == 23);
		txt.resize(23);
		CHECK(txt == std::string("2.345678806304931640625"));
		txt.resize(9);
		used = ToString(txt.begin(), txt.end(), 0.3456789f, 10);
		CHECK(used == 9);
		CHECK(txt == std::string("0.3456789"));
	}

	TEST_CASE("float exp notation") {
		std::string txt{};
		txt.resize(100);
		auto used = ToString(txt.begin(), txt.end(), 100.0e10f, 10);
		CHECK(used == 4);
		txt.resize(used);
		CHECK(txt == std::string("1e12"));
		txt.resize(8);
		used = ToString(txt.begin(), txt.end(), 999.99e10f, 10);
		CHECK(used == 8);
		txt.resize(used);
		CHECK(txt == std::string("9.999e12"));
		used = ToString(txt.begin(), txt.end(), 100.0e-14f, 10);
		CHECK(used == 5);
		txt.resize(used);
		CHECK(txt == std::string("1e-12"));
		txt.resize(9);
		used = ToString(txt.begin(), txt.end(), 999.99e-14f, 10);
		CHECK(used == 9);
		txt.resize(used);
		CHECK(txt == std::string("9.999e-12"));
	}

	TEST_CASE("sign test") {
		std::string txt{};
		txt.resize(100);
		auto used = ToString(txt.begin(), txt.end(), -125.0e10f, 10);
		CHECK(used == 8);
		txt.resize(8);
		CHECK(txt == std::string("-1.25e12"));
		used = ToString(txt.begin(), txt.end(), -1, 10);
		CHECK(used == 2);
		txt.resize(used);
		CHECK(txt == std::string("-1"));
	}

	TEST_CASE("base") {
		std::string txt{};
		txt.resize(100);
		auto used = ToString(txt.begin(), txt.end(), 0xF, 16);
		CHECK(used == 1);
		txt.resize(used);
		CHECK(txt == std::string("F"));
		txt.resize(100);
		used = ToString(txt.begin(), txt.end(), float(0x38D7C50000000LL), 16);
		CHECK(used == 9);
		txt.resize(used);
		CHECK(txt == std::string("3.8D7C5pC"));
		txt.resize(100);
		used = ToString(txt.begin(), txt.end(), float(-0x38D7C50000000LL), 16);
		CHECK(used == 10);
		txt.resize(used);
		CHECK(txt == std::string("-3.8D7C5pC"));
		txt.resize(100);
		used = ToString(txt.begin(), txt.end(), float(0x38D7C50000000LL), 2);
		CHECK(used == 30);
		txt.resize(used);
		CHECK(txt == std::string("1.110001101011111000101e110001"));
		txt.resize(100);
		used = ToString(txt.begin(), txt.end(), float(0x1000LL), 16);
		CHECK(used == 4);
		txt.resize(used);
		CHECK(txt == std::string("1000"));
	}
}
