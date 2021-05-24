/*
 * test_pid.cpp
 *
 *  Created on: 08.01.2021
 *      Author: sebastian
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#include "pid.hpp"

TEST_SUITE("pid") {
    using utility::controller::CPidControl;

    template <typename T>
    void CheckSetters() {
        CPidControl<T> regulator{{1, 2, 3}};
        typename CPidControl<T>::SParameterSet params{1, 2, 3};  // NOLINT
        typename CPidControl<T>::SLimitSet limits{-100, 100};    // NOLINT
        regulator.SetParameters(params);
        regulator.SetLimits(limits);
        regulator.SetSetPoint(1);

        auto cmp_params = regulator.GetParameters();
        CHECK(cmp_params.m_proportional_part == params.m_proportional_part);
        CHECK(cmp_params.m_integral_part == params.m_integral_part);
        CHECK(cmp_params.m_derivative_part == params.m_derivative_part);

        auto cmp_limits = regulator.GetLimits();
        CHECK(cmp_limits.m_lower_bound == limits.m_lower_bound);
        CHECK(cmp_limits.m_upper_bound == limits.m_upper_bound);
        CHECK(regulator.GetSetPoint() == 1);
    }
    TEST_CASE("setters") {
        CheckSetters<int>();
        CheckSetters<float>();
    }

    template <typename T>
    void CheckProportionalPart() {
        CPidControl<T> regulator{{1, 0, 0}};
        regulator.SetSetPoint(0);
        CHECK(regulator.ComputeOutput(-1) == 1);
        CHECK(regulator.ComputeOutput(0) == 0);

        CPidControl<T> regulator2{{2, 0, 0}};
        regulator2.SetSetPoint(0);
        CHECK(regulator2.ComputeOutput(-1) == 2);
        CHECK(regulator2.ComputeOutput(0) == 0);
    }
    TEST_CASE("proportional part") {
        CheckProportionalPart<int>();
        CheckProportionalPart<float>();
    }

    template <typename T>
    void CheckIntegralPart() {
        CPidControl<T> regulator{{0, 1, 0}};
        regulator.SetSetPoint(0);
        CHECK(regulator.ComputeOutput(-1) == 1);
        CHECK(regulator.ComputeOutput(0) == 1);

        CPidControl<T> regulator2{{0, 2, 0}};
        regulator2.SetSetPoint(0);
        CHECK(regulator2.ComputeOutput(-2) == 4);
        CHECK(regulator2.ComputeOutput(-1) == 6);
        CHECK(regulator2.ComputeOutput(0) == 6);
    }
    TEST_CASE("integral part") {
        CheckIntegralPart<int>();
        CheckIntegralPart<float>();
    }

    template <typename T>
    void CheckDerivativePart() {
        CPidControl<T> regulator{{0, 0, 1}};
        regulator.SetSetPoint(0);
        CHECK(regulator.ComputeOutput(-1) == 1);
        CHECK(regulator.ComputeOutput(0) == -1);

        CPidControl<T> regulator2{{0, 0, 2}};
        regulator2.SetSetPoint(0);
        CHECK(regulator2.ComputeOutput(-2) == 4);
        CHECK(regulator2.ComputeOutput(-1) == -2);
        CHECK(regulator2.ComputeOutput(0) == -2);
    }
    TEST_CASE("derivative part") {
        CheckDerivativePart<int>();
        CheckDerivativePart<float>();
    }

    template <typename T>
    void CheckLimitsPart() {
        typename CPidControl<T>::SLimitSet limits{0, 1};
        CPidControl<T> p_regulator{{2, 0, 0}, limits};
        p_regulator.SetSetPoint(0);
        CHECK(p_regulator.ComputeOutput(-1) == 1);
        CHECK(p_regulator.ComputeOutput(1) == 0);

        CPidControl<T> i_regulator{{0, 1, 0}, limits};
        i_regulator.SetSetPoint(0);
        CHECK(i_regulator.ComputeOutput(-2) == 1);
        CHECK(i_regulator.ComputeOutput(2) == 0);

        CPidControl<T> d_regulator{{0, 0, 1}, limits};
        d_regulator.SetSetPoint(0);
        CHECK(d_regulator.ComputeOutput(-2) == 1);
        CHECK(d_regulator.ComputeOutput(0) == 0);
    }
    TEST_CASE("limits") {
        CheckLimitsPart<int>();
        CheckLimitsPart<float>();
    }

    template <typename T>
    void CheckWindupPart() {
        CPidControl<T> i_regulator{{0, 1, 1}, {0, 8}};  // NOLINT
        i_regulator.SetSetPoint(0);
        CHECK(i_regulator.ComputeOutput(-8) == 8);
        CHECK(i_regulator.ComputeOutput(-8) == 8);
        CHECK(i_regulator.ComputeOutput(-3) == 6);
        CHECK(i_regulator.ComputeOutput(-1) == 8);
        CHECK(i_regulator.ComputeOutput(0) == 8);
        CHECK(i_regulator.ComputeOutput(1) == 8);
        CHECK(i_regulator.ComputeOutput(1) == 8);
        CHECK(i_regulator.ComputeOutput(1) == 8);
        CHECK(i_regulator.ComputeOutput(1) == 7);
        CHECK(i_regulator.ComputeOutput(1) == 6);

        CPidControl<T> i_regulator2{{0, 1, 1}, {-8, 0}};  // NOLINT
        i_regulator2.SetSetPoint(0);
        CHECK(i_regulator2.ComputeOutput(8) == -8);
        CHECK(i_regulator2.ComputeOutput(8) == -8);
        CHECK(i_regulator2.ComputeOutput(3) == -6);
        CHECK(i_regulator2.ComputeOutput(1) == -8);
        CHECK(i_regulator2.ComputeOutput(0) == -8);
        CHECK(i_regulator2.ComputeOutput(-1) == -8);
        CHECK(i_regulator2.ComputeOutput(-1) == -8);
        CHECK(i_regulator2.ComputeOutput(-1) == -8);
        CHECK(i_regulator2.ComputeOutput(-1) == -7);
        CHECK(i_regulator2.ComputeOutput(-1) == -6);
    }
    TEST_CASE("windup") {
        CheckWindupPart<int>();
        CheckWindupPart<float>();
    }
}
