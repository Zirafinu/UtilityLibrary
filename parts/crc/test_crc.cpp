/*
 * test_crc.cpp
 *
 *  Created on: 08.01.2021
 *      Author: sebastian
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include "crc.hpp"

TEST_SUITE("CRC"){
	using utility::crc::CCrc;
	using utility::crc::CCrcBase;

	template<uint16_t value, uint_fast8_t tblBits>
	void CheckCRC_initialValue() {
		CCrc<CCrcBase<uint16_t, 0xFFFF, tblBits>, value, 0, false> crc{};
		CHECK(crc.GetCRC() == value);
	}
	TEST_CASE("Initial Values") {
		CheckCRC_initialValue<0, 1>();
		CheckCRC_initialValue<0x8001, 1>();
		CheckCRC_initialValue<2, 1>();

		CheckCRC_initialValue<0, 2>();
		CheckCRC_initialValue<0x8002, 4>();
		CheckCRC_initialValue<3, 8>();
	}

	template<uint16_t value, uint_fast8_t tblBits>
	void CheckCRC_xorValue() {
		CCrc<CCrcBase<uint16_t, 0xFFFF, tblBits>, 0, value, false> crc{};
		CHECK(crc.GetCRC() == value);
	}
	TEST_CASE("XOR Values") {
		CheckCRC_xorValue<1, 1>();
		CheckCRC_xorValue<0x8003, 1>();
		CheckCRC_xorValue<4, 1>();

		CheckCRC_xorValue<6, 2>();
		CheckCRC_xorValue<0x8000, 4>();
		CheckCRC_xorValue<1, 8>();
	}

	template<uint_fast8_t tblBits>
	void Check_singleByte()
	{
		CCrc<CCrcBase<uint16_t,0x8005, tblBits,false> , 0, 0> crc {};
		crc.AddData(0);
		CHECK(crc.GetCRC() == 0);
		crc.AddData('1');
		CHECK(crc.GetCRC() == 0x80A5);
		crc.AddData(crc.GetCRC());
		CHECK(crc.GetCRC() == 0);
	}
	TEST_CASE("Single Byte") {
		Check_singleByte<1>();
		Check_singleByte<2>();
		Check_singleByte<4>();
		Check_singleByte<8>();
	}

	template<uint_fast8_t tblBits>
	void Check_123456789String()
	{
		CCrc<CCrcBase<uint32_t, 0x04C11DB7, tblBits, true>, 0xFFffFFff, 0xFFffFFff, true> crc {};
		const std::array<uint8_t, 9> data {'1','2','3','4','5','6','7','8','9'};

		crc.AddData(data.cbegin(), data.cend());
		CHECK(crc.GetCRC() == 0xCBF43926);
	}
	TEST_CASE("Crc over String") {
		Check_123456789String<1>();
		Check_123456789String<2>();
		Check_123456789String<4>();
		Check_123456789String<8>();
	}
}

