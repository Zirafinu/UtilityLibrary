/*
 * pid.hpp
 *
 *  Created on: 08.01.2021
 *      Author: sebastian
 */

#ifndef PID_HPP
#define PID_HPP

#include <numeric>

namespace utility::controler {
template<typename InputType, typename OutputType = InputType,
		typename ComputationType = OutputType>
class PID_Control {
public:
	struct ParameterSet {
		ComputationType proportional_part;
		ComputationType integral_part;
		ComputationType derivative_part;
	};
	struct LimitSet {
		OutputType lower_bound;
		OutputType upper_bound;
	};
public:
	PID_Control(const ParameterSet &params = {},
			const LimitSet &limits = LimitSet {
					std::numeric_limits<OutputType>::lowest(), std::numeric_limits<
							OutputType>::max() }) :
			parameters(params), limits(limits) {
	}

	void setParameters(const ParameterSet &parameters) noexcept {
		this->parameters = parameters;
	}
	void setLimits(const LimitSet &limits) noexcept {
		this->limits = limits;
	}
	void setSetPoint(const OutputType& value) noexcept {
		setpoint = value;
	}

	[[nodiscard]] auto getParameters() const noexcept -> const ParameterSet& {
		return parameters;
	}
	[[nodiscard]] auto getLimits() const noexcept -> const LimitSet& {
		return limits;
	}
	[[nodiscard]] auto getSetPoint() const noexcept -> const OutputType& {
		return setpoint;
	}

	OutputType ComputeOutput(const InputType & input) {
		const ComputationType error = (setpoint - input);

		const ComputationType ppart = error * parameters.proportional_part;
		const ComputationType ipart = (integral + error) * parameters.integral_part;
		const ComputationType dpart = (error - last_error) * parameters.derivative_part;
		last_error = error;

		const OutputType output = ppart + ipart + dpart;
		if(output > limits.upper_bound) {
			if(error < 0) integral += error;
			return limits.upper_bound;
		}
		if(output < limits.lower_bound) {
			if(error > 0) integral += error;
			return limits.lower_bound;
		}

		integral += error;
		return output;
	}
private:
	ComputationType last_error{0};
	ComputationType integral {0};
	OutputType setpoint {0};
	ParameterSet parameters;
	LimitSet limits;
};
}

#endif /* PID_HPP */
