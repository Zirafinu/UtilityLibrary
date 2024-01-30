/*
 * test_parameters.cpp
 *
 *  Created on: 08.01.2021
 *      Author: sebastian
 */

#include <numeric>

#include "doctest/doctest.h"
#include "parameters.hpp"
#include "parameters_converters.hpp"
#include "parameters_table.hpp"

constexpr std::size_t GC_SERIAL_NUMBER_LENGTH = 8;
using SerialType = std::array<char, GC_SERIAL_NUMBER_LENGTH>;
using SerialParam = utility::parameters::CPlainParameter<0, SerialType>;
using SerialParam1 = utility::parameters::CPlainParameter<1, SerialType>;
using SerialParam2 = utility::parameters::CPlainParameter<2, SerialType>;
using SerialValueConverter = utility::parameters::CFixedArrayConverter<SerialType>;

static const SerialValueConverter GC_SERIAL_CONVERTER{};

template <>
auto SerialParam::GetInstance() noexcept -> ThisType& {
    static SerialParam s_param{"serial", "the serial",
                               SerialType{'0', '0', '0', '0', '0', '0', '0', '0'},
                               GC_SERIAL_CONVERTER};
    return s_param;
}
template <>
auto SerialParam1::GetInstance() noexcept -> ThisType& {
    static ThisType s_param{"serial1", "the serial",
                            SerialType{'0', '0', '0', '0', '0', '0', '0', '1'},
                            GC_SERIAL_CONVERTER};
    return s_param;
}
template <>
auto SerialParam2::GetInstance() noexcept -> ThisType& {
    static ThisType s_param{"serial2", "the serial",
                            SerialType{'0', '0', '0', '0', '0', '0', '0', '2'},
                            GC_SERIAL_CONVERTER};
    return s_param;
}

template <typename T>
std::string ToString(const T& array) {
    return std::accumulate(std::begin(array), std::end(array), std::string{}, [](auto& acc, const auto& v){return acc + static_cast<char>(v);});
}

TEST_SUITE("parameters") {
    utility::parameters::CCParametersTableImpl<SerialParam, SerialParam1, SerialParam2>
        G_PARAMETERS;

    TEST_CASE("initial serial") {
        auto tmp = (*G_PARAMETERS.GetTableBegin())->GetValueBytes();
        const std::string cmp{"00000000"};
        CHECK(ToString(tmp) == cmp);
    }

    TEST_CASE("serial 1-8") {
        const std::string txt = "12345678";
        CHECK(ToString(SerialParam::GetInstance().ParseValueFromString(txt)) ==
              ToString(SerialParam::NO_ERROR));
        CHECK(ToString(SerialParam::Value()) == txt);
    }
}
