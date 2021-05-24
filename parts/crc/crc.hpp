/*
 * CCrc8.hpp
 *
 *  Created on: 11.12.2020
 *      Author: sebastian
 */

#ifndef CRC_HPP
#define CRC_HPP

#include <array>
#include <bit_operations.hpp>
#include <climits>
#include <cstdint>

namespace utility::crc {

template <typename T, T POLYNOM, std::size_t BITS_IN_TABLE, bool REFLECT_DATA>
class CCrcBase {
    static_assert(!std::is_signed<T>::value);
    static_assert(std::is_integral<T>::value);
    static_assert(
        BITS_IN_TABLE == CHAR_BIT || BITS_IN_TABLE == CHAR_BIT / 2 ||
        BITS_IN_TABLE ==
            2);  // NOLINT(readability-magic-numbers,cppcoreguidelines-avoid-magic-numbers)

    [[nodiscard]] constexpr static auto ComputeTable() -> std::array<T, 1U << BITS_IN_TABLE> {
        std::array<T, 1U << BITS_IN_TABLE> table{0U};
        for (std::size_t j = 0; j < table.size(); ++j) {
            T value = 0;
            for (int i = BITS_IN_TABLE - 1; i >= 0; --i) {
                uint8_t selected_bit = (value >> (BIT_LENGTH - 1U));
                uint8_t value_bit = ((j >> unsigned(i)) & 0x1U);

                value <<= 1U;
                if ((selected_bit ^ value_bit) != 0) {
                    value ^= POLYNOM;
                }
            }
            table[j] = value;  // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
        }
        return table;
    }

    constexpr inline static const std::size_t BIT_LENGTH{sizeof(T) * CHAR_BIT};
    constexpr inline static const auto TABLE_BITS_MASK = (1U << BITS_IN_TABLE) - 1U;
    constexpr inline static const auto TABLE = ComputeTable();

   public:
    using ValueType = T;

    constexpr CCrcBase() = default;

    [[nodiscard]] constexpr static auto AppendByte(uint8_t value, ValueType crc_value) noexcept
        -> ValueType {
        if constexpr (REFLECT_DATA) {
            value = utility::general::bit_ops::RevertBits(value);
        }
        for (int8_t i = CHAR_BIT - BITS_IN_TABLE; i >= 0; i -= BITS_IN_TABLE) {
            uint8_t selected_bits = crc_value >> (BIT_LENGTH - BITS_IN_TABLE);
            uint8_t value_bits = value >> unsigned(i);
            crc_value <<= BITS_IN_TABLE;
            // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
            crc_value ^= TABLE[unsigned(selected_bits ^ value_bits) & TABLE_BITS_MASK];
        }
        return crc_value;
    }
};

template <typename T, T POLYNOM, bool REFLECT_DATA>
class CCrcBase<T, POLYNOM, 1, REFLECT_DATA> {
    static_assert(!std::is_signed<T>::value);
    static_assert(std::is_integral<T>::value);

    constexpr inline static const std::size_t BIT_LENGTH = sizeof(T) * CHAR_BIT;

   public:
    using ValueType = T;

    constexpr CCrcBase() = default;

    [[nodiscard]] constexpr static auto AppendByte(uint8_t value, ValueType crc_value) noexcept
        -> ValueType {
        if constexpr (REFLECT_DATA) {
            crc_value ^= ValueType(utility::general::bit_ops::RevertBits(value))
                         << (BIT_LENGTH - CHAR_BIT);
        } else {
            crc_value ^= ValueType(value) << (BIT_LENGTH - CHAR_BIT);
        }
        for (uint8_t i = CHAR_BIT; i > 0; --i) {
            if ((crc_value & (1U << (BIT_LENGTH - 1U))) != 0) {
                crc_value = (crc_value << 1U) ^ POLYNOM;
            } else {
                crc_value = (crc_value << 1U);
            }
        }
        return crc_value;
    }
};

template <typename T = CCrcBase<uint16_t, 0x8005U,  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
                                CHAR_BIT, false>,
          typename T::ValueType INITIAL = 0, typename T::ValueType XOR_VALUE = 0,
          bool REFLECT_REMAINDER = false>
class CCrc {
    using ValueType = typename T::ValueType;
    T m_ccrc_base{};
    ValueType m_crc_value{INITIAL};

   public:
    constexpr CCrc() = default;

    template <typename U>
    constexpr void AddData(U value) noexcept {
        static_assert(std::is_integral<U>::value);
        for (std::size_t i = sizeof(U); i > 0; --i) {
            m_crc_value =
                m_ccrc_base.AppendByte(uint8_t(value >> unsigned((i - 1) * CHAR_BIT)), m_crc_value);
        }
    }

    constexpr void AddData(const uint8_t *start, const uint8_t *end) noexcept {
        while (start != end) {
            m_crc_value = m_ccrc_base.AppendByte(
                *start++, m_crc_value);  // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        }
    }

    constexpr void AddData(const uint8_t *start, std::size_t length) noexcept {
        AddData(start, start + length);  // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    }

    [[nodiscard]] constexpr auto GetCRC() const noexcept -> ValueType {
        if constexpr (REFLECT_REMAINDER) {
            return utility::general::bit_ops::RevertBits(m_crc_value) ^ XOR_VALUE;
        } else {
            return m_crc_value ^ XOR_VALUE;
        }
    }
};

constexpr uint16_t CRC_16_IBM_POLINOM = 0x8005U;
template <std::size_t BITS_IN_TABLE>
using CCrc16IBM = CCrc<CCrcBase<uint16_t, CRC_16_IBM_POLINOM, BITS_IN_TABLE, false>, 0, 0, false>;

constexpr uint32_t CRC_32_POLINOM = 0x04C11DB7U;
constexpr uint32_t CRC_32_INITIAL = 0xFFffFFffU;
constexpr uint32_t CRC_32_XOR = 0xFFffFFffU;
template <std::size_t BITS_IN_TABLE>
using CCrc32 =
    CCrc<CCrcBase<uint32_t, CRC_32_POLINOM, BITS_IN_TABLE, true>, CRC_32_INITIAL, CRC_32_XOR, true>;

} /* namespace utility::crc */

#endif /* CRC_HPP */
