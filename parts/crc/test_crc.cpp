/*
 * test_crc.cpp
 *
 *  Created on: 08.01.2021
 *      Author: sebastian
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "crc.hpp"
#include "doctest/doctest.h"

TEST_SUITE("CRC") {
    using utility::crc::CCrc;
    using utility::crc::CCrc16IBM;
    using utility::crc::CCrc32;
    using utility::crc::CCrcBase;
    constexpr uint16_t EASY_POLI = 0xFFFFU;

    template <uint16_t VALUE, uint_fast8_t TABLE_BITS>
    void CheckCRCInitialValue() {
        CCrc<CCrcBase<uint16_t, EASY_POLI, TABLE_BITS, false>, VALUE, 0, false> crc{};  // NOLINT
        CHECK(crc.GetCRC() == VALUE);
    }
    TEST_CASE("Initial Values") {
        CheckCRCInitialValue<0, 1>();
        CheckCRCInitialValue<0x8001, 1>();  // NOLINT
        CheckCRCInitialValue<2, 1>();

        CheckCRCInitialValue<0, 2>();
        CheckCRCInitialValue<0x8002, 4>();  // NOLINT
        CheckCRCInitialValue<3, 8>();       // NOLINT
    }

    template <uint16_t VALUE, uint_fast8_t TABLE_BITS>
    void CheckCRCXorValue() {
        CCrc<CCrcBase<uint16_t, EASY_POLI, TABLE_BITS, false>, 0, VALUE, false> crc{};  // NOLINT
        CHECK(crc.GetCRC() == VALUE);
    }
    TEST_CASE("XOR Values") {
        CheckCRCXorValue<1, 1>();
        CheckCRCXorValue<0x8003, 1>();  // NOLINT
        CheckCRCXorValue<4, 1>();

        CheckCRCXorValue<6, 2>();       // NOLINT
        CheckCRCXorValue<0x8000, 4>();  // NOLINT
        CheckCRCXorValue<1, 8>();       // NOLINT
    }

    template <uint_fast8_t TABLE_BITS>
    void CheckSingleByte() {
        CCrc16IBM<TABLE_BITS> crc{};
        crc.AddData(0);
        CHECK(crc.GetCRC() == 0);
        crc.AddData('1');
        CHECK(crc.GetCRC() == 0x80A5);
        crc.AddData(crc.GetCRC());
        CHECK(crc.GetCRC() == 0);
    }
    TEST_CASE("Single Byte") {
        CheckSingleByte<1>();
        CheckSingleByte<2>();
        CheckSingleByte<4>();
        CheckSingleByte<8>();  // NOLINT
    }

    template <uint_fast8_t TABLE_BITS>
    void Check123456789String() {
        CCrc32<TABLE_BITS> crc{};
        const std::array<uint8_t, 9> data{'1', '2', '3', '4', '5', '6', '7', '8', '9'};

        crc.AddData(data.cbegin(), data.cend());
        CHECK(crc.GetCRC() == 0xCBF43926);
    }
    TEST_CASE("Crc over String") {
        Check123456789String<1>();
        Check123456789String<2>();
        Check123456789String<4>();
        Check123456789String<8>();  // NOLINT
    }
}
