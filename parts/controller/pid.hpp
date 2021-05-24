/*
 * pid.hpp
 *
 *  Created on: 08.01.2021
 *      Author: sebastian
 */

#ifndef PID_HPP
#define PID_HPP

#include <limits>
#include <type_traits>

namespace utility::controller {
template <typename INPUT_TYPE, typename OUTPUT_TYPE = INPUT_TYPE,
          typename COMPUTATION_TYPE = typename std::common_type<INPUT_TYPE, OUTPUT_TYPE>::type>
class CPidControl {
   public:
    struct SParameterSet {
        COMPUTATION_TYPE m_proportional_part;
        COMPUTATION_TYPE m_integral_part;
        COMPUTATION_TYPE m_derivative_part;
    };
    struct SLimitSet {
        OUTPUT_TYPE m_lower_bound{std::numeric_limits<OUTPUT_TYPE>::lowest()};
        OUTPUT_TYPE m_upper_bound{std::numeric_limits<OUTPUT_TYPE>::max()};
    };

    explicit CPidControl(const SParameterSet& params, const SLimitSet& limits = SLimitSet{});

    void SetParameters(const SParameterSet& parameters) noexcept;
    void SetLimits(const SLimitSet& limits) noexcept;
    void SetSetPoint(const OUTPUT_TYPE& value) noexcept;

    [[nodiscard]] auto GetParameters() const noexcept -> const SParameterSet&;
    [[nodiscard]] auto GetLimits() const noexcept -> const SLimitSet&;
    [[nodiscard]] auto GetSetPoint() const noexcept -> const OUTPUT_TYPE&;

    OUTPUT_TYPE ComputeOutput(const INPUT_TYPE& input);

   private:
    COMPUTATION_TYPE m_last_error{0};
    COMPUTATION_TYPE m_integral{0};
    OUTPUT_TYPE m_setpoint{0};
    SParameterSet m_parameters;
    SLimitSet m_limits;
};

template <typename INPUT_TYPE, typename OUTPUT_TYPE, typename COMPUTATION_TYPE>
CPidControl<INPUT_TYPE, OUTPUT_TYPE, COMPUTATION_TYPE>::CPidControl(const SParameterSet& params,
                                                                    const SLimitSet& limits)
    : m_parameters(params), m_limits(limits) {}

template <typename INPUT_TYPE, typename OUTPUT_TYPE, typename COMPUTATION_TYPE>
void CPidControl<INPUT_TYPE, OUTPUT_TYPE, COMPUTATION_TYPE>::SetParameters(
    const SParameterSet& parameters) noexcept {
    m_parameters = parameters;
}

template <typename INPUT_TYPE, typename OUTPUT_TYPE, typename COMPUTATION_TYPE>
void CPidControl<INPUT_TYPE, OUTPUT_TYPE, COMPUTATION_TYPE>::SetLimits(
    const SLimitSet& limits) noexcept {
    m_limits = limits;
}

template <typename INPUT_TYPE, typename OUTPUT_TYPE, typename COMPUTATION_TYPE>
void CPidControl<INPUT_TYPE, OUTPUT_TYPE, COMPUTATION_TYPE>::SetSetPoint(
    const OUTPUT_TYPE& value) noexcept {
    m_setpoint = value;
}

template <typename INPUT_TYPE, typename OUTPUT_TYPE, typename COMPUTATION_TYPE>
[[nodiscard]] auto CPidControl<INPUT_TYPE, OUTPUT_TYPE, COMPUTATION_TYPE>::GetParameters()
    const noexcept -> const SParameterSet& {
    return m_parameters;
}

template <typename INPUT_TYPE, typename OUTPUT_TYPE, typename COMPUTATION_TYPE>
[[nodiscard]] auto CPidControl<INPUT_TYPE, OUTPUT_TYPE, COMPUTATION_TYPE>::GetLimits()
    const noexcept -> const SLimitSet& {
    return m_limits;
}

template <typename INPUT_TYPE, typename OUTPUT_TYPE, typename COMPUTATION_TYPE>
[[nodiscard]] auto CPidControl<INPUT_TYPE, OUTPUT_TYPE, COMPUTATION_TYPE>::GetSetPoint()
    const noexcept -> const OUTPUT_TYPE& {
    return m_setpoint;
}

template <typename INPUT_TYPE, typename OUTPUT_TYPE, typename COMPUTATION_TYPE>
OUTPUT_TYPE CPidControl<INPUT_TYPE, OUTPUT_TYPE, COMPUTATION_TYPE>::ComputeOutput(
    const INPUT_TYPE& input) {
    const COMPUTATION_TYPE error = (m_setpoint - input);

    const COMPUTATION_TYPE ppart = error * m_parameters.m_proportional_part;
    const COMPUTATION_TYPE ipart = (m_integral + error) * m_parameters.m_integral_part;
    const COMPUTATION_TYPE dpart = (error - m_last_error) * m_parameters.m_derivative_part;
    m_last_error = error;

    const OUTPUT_TYPE output = ppart + ipart + dpart;
    if (output > m_limits.m_upper_bound) {
        if (error < 0) {
            m_integral += error;
        }
        return m_limits.m_upper_bound;
    }
    if (output < m_limits.m_lower_bound) {
        if (error > 0) {
            m_integral += error;
        }
        return m_limits.m_lower_bound;
    }

    m_integral += error;
    return output;
}

}  // namespace utility::controller

#endif /* PID_HPP */
