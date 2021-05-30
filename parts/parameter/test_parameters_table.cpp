/*
 * test_parameters_table.cpp
 *
 *  Created on: 24.05.2021
 *      Author: sebastian
 */

#include <iterator>

//#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include "parameters.hpp"
#include "parameters_converters.hpp"
#include "parameters_table.hpp"

using Param = utility::parameters::CPlainParameter<0, int>;
using Param1 = utility::parameters::CPlainParameter<1, int>;
using Param2 = utility::parameters::CPlainParameter<2, int>;
using ValueConverter = utility::parameters::CNumberConverter<int>;

static const ValueConverter GC_CONVERTER{};

template <>
auto Param::GetInstance() noexcept -> ThisType& {
    static Param s_param{"", "", 0, GC_CONVERTER};
    return s_param;
}
template <>
auto Param1::GetInstance() noexcept -> ThisType& {
    static ThisType s_param{"", "", 1, GC_CONVERTER};
    return s_param;
}
template <>
auto Param2::GetInstance() noexcept -> ThisType& {
    static ThisType s_param{"", "", 2, GC_CONVERTER};
    return s_param;
}

TEST_SUITE("parameters") {
    utility::parameters::CCParametersTableImpl<Param, Param1, Param2> G_PARAMETERS;
    TEST_CASE("first param") {
        CHECK(std::size_t(*G_PARAMETERS.GetTableBegin()) == std::size_t(&Param::GetInstance()));
    }

    TEST_CASE("third param") {
        CHECK(std::size_t(*std::next(G_PARAMETERS.GetTableBegin(), 2)) ==
              std::size_t(&Param2::GetInstance()));
    }

    TEST_CASE("list length") {
        CHECK(std::size_t(std::next(G_PARAMETERS.GetTableBegin(), 3)) ==
              std::size_t(G_PARAMETERS.GetTableEnd()));
    }
}
