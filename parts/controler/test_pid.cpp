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
	using utility::controler::PID_Control;

	template<typename T>
	void Check_setters() {
		PID_Control<T> regulator{};
		typename PID_Control<T>::ParameterSet params{1,2,3};
		typename PID_Control<T>::LimitSet limits{-100, 100};
		regulator.setParameters(params);
		regulator.setLimits(limits);
		regulator.setSetPoint(1);

		auto cmpParams = regulator.getParameters();
		CHECK(cmpParams.proportional_part == params.proportional_part);
		CHECK(cmpParams.integral_part == params.integral_part);
		CHECK(cmpParams.derivative_part == params.derivative_part);

		auto cmpLimits = regulator.getLimits();
		CHECK(cmpLimits.lower_bound == limits.lower_bound);
		CHECK(cmpLimits.upper_bound == limits.upper_bound);
		CHECK(regulator.getSetPoint() == 1);
	}
	TEST_CASE("setters") {
		Check_setters<int>();
		Check_setters<float>();
	}

	template<typename T>
	void Check_proportionalPart(){
		PID_Control<T> regulator{{1, 0, 0}};
		regulator.setSetPoint(0);
		CHECK(regulator.ComputeOutput(-1) == 1);
		CHECK(regulator.ComputeOutput(0) == 0);

		PID_Control<T> regulator2{{2, 0, 0}};
		regulator2.setSetPoint(0);
		CHECK(regulator2.ComputeOutput(-1) == 2);
		CHECK(regulator2.ComputeOutput(0) == 0);
	}
	TEST_CASE("proportional part"){
		Check_proportionalPart<int>();
		Check_proportionalPart<float>();
	}

	template<typename T>
	void Check_integralPart(){
		PID_Control<T> regulator{{0, 1, 0}};
		regulator.setSetPoint(0);
		CHECK(regulator.ComputeOutput(-1) == 1);
		CHECK(regulator.ComputeOutput(0) == 1);

		PID_Control<T> regulator2{{0, 2, 0}};
		regulator2.setSetPoint(0);
		CHECK(regulator2.ComputeOutput(-2) == 4);
		CHECK(regulator2.ComputeOutput(-1) == 6);
		CHECK(regulator2.ComputeOutput(0) == 6);
	}
	TEST_CASE("integral part"){
		Check_integralPart<int>();
		Check_integralPart<float>();
	}

	template<typename T>
	void Check_derivativePart(){
		PID_Control<T> regulator{{0, 0, 1}};
		regulator.setSetPoint(0);
		CHECK(regulator.ComputeOutput(-1) == 1);
		CHECK(regulator.ComputeOutput(0) == -1);

		PID_Control<T> regulator2{{0, 0, 2}};
		regulator2.setSetPoint(0);
		CHECK(regulator2.ComputeOutput(-2) == 4);
		CHECK(regulator2.ComputeOutput(-1) == -2);
		CHECK(regulator2.ComputeOutput(0) == -2);
	}
	TEST_CASE("derivative part"){
		Check_derivativePart<int>();
		Check_derivativePart<float>();
	}

	template<typename T>
	void Check_limitsPart(){
		PID_Control<T> p_regulator{{2,0,0}, {0, 1}};
		p_regulator.setSetPoint(0);
		CHECK(p_regulator.ComputeOutput(-1) == 1);
		CHECK(p_regulator.ComputeOutput(0) == 0);

		PID_Control<T> i_regulator{{0, 1, 0}, {0, 1}};
		i_regulator.setSetPoint(0);
		CHECK(i_regulator.ComputeOutput(-2) == 1);
		CHECK(i_regulator.ComputeOutput(-1) == 1);
		CHECK(i_regulator.ComputeOutput(0) == 1);

		PID_Control<T> d_regulator{{0, 0, 1}, {0, 1}};
		d_regulator.setSetPoint(0);
		CHECK(d_regulator.ComputeOutput(-2) == 1);
		CHECK(d_regulator.ComputeOutput(-1) == 0);
		CHECK(d_regulator.ComputeOutput(0) == 0);
	}
	TEST_CASE("limits"){
		Check_limitsPart<int>();
		Check_limitsPart<float>();
	}

	template<typename T>
	void Check_windupPart() {
		PID_Control<T> i_regulator{{0, 1, 1}, {0, 8}};
		i_regulator.setSetPoint(0);
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

		PID_Control<T> i_regulator2{{0, 1, 1}, {-8, 0}};
		i_regulator2.setSetPoint(0);
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
		Check_windupPart<int>();
		Check_windupPart<float>();
	}
}
