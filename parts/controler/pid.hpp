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
template<typename INPUT_TYPE, typename OUTPUT_TYPE = INPUT_TYPE,
		typename COMPUTATION_TYPE = OUTPUT_TYPE>
class CPidControl {
public:
	struct SParameterSet {
		COMPUTATION_TYPE m_proportional_part;
		COMPUTATION_TYPE m_integral_part;
		COMPUTATION_TYPE m_derivative_part;
	};
	struct SLimitSet {
		OUTPUT_TYPE m_lower_bound;
		OUTPUT_TYPE m_upper_bound;
	};

	explicit CPidControl(const SParameterSet &params = {},
			const SLimitSet &limits = SLimitSet {
					std::numeric_limits<OUTPUT_TYPE>::lowest(), std::numeric_limits<
							OUTPUT_TYPE>::max() }) :
			m_parameters(params), m_limits(limits) {
	}

	void SetParameters(const SParameterSet &parameters) noexcept {
		this->m_parameters = parameters;
	}
	void SetLimits(const SLimitSet &limits) noexcept {
		this->m_limits = limits;
	}
	void SetSetPoint(const OUTPUT_TYPE& value) noexcept {
		m_setpoint = value;
	}

	[[nodiscard]] auto GetParameters() const noexcept -> const SParameterSet& {
		return m_parameters;
	}
	[[nodiscard]] auto GetLimits() const noexcept -> const SLimitSet& {
		return m_limits;
	}
	[[nodiscard]] auto GetSetPoint() const noexcept -> const OUTPUT_TYPE& {
		return m_setpoint;
	}

	OUTPUT_TYPE ComputeOutput(const INPUT_TYPE & input) {
		const COMPUTATION_TYPE error = (m_setpoint - input);

		const COMPUTATION_TYPE ppart = error * m_parameters.m_proportional_part;
		const COMPUTATION_TYPE ipart = (m_integral + error) * m_parameters.m_integral_part;
		const COMPUTATION_TYPE dpart = (error - m_last_error) * m_parameters.m_derivative_part;
		m_last_error = error;

		const OUTPUT_TYPE output = ppart + ipart + dpart;
		if(output > m_limits.m_upper_bound) {
			if(error < 0) {
				m_integral += error;
			}
			return m_limits.m_upper_bound;
		}
		if(output < m_limits.m_lower_bound) {
			if(error > 0) {
				m_integral += error;
			}
			return m_limits.m_lower_bound;
		}

		m_integral += error;
		return output;
	}
private:
	COMPUTATION_TYPE m_last_error{0};
	COMPUTATION_TYPE m_integral {0};
	OUTPUT_TYPE m_setpoint {0};
	SParameterSet m_parameters;
	SLimitSet m_limits;
};
}// namespace utility::controler

#endif /* PID_HPP */
