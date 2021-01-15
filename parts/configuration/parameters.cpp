/*
 * ParametersBase.cpp
 *
 *  Created on: 04.12.2020
 *      Author: sebastian
 */

#include "parameters.hpp"

namespace utility::parameters {

auto CAParameter::GetID() const -> uint16_t {
	return mc_id;
}
auto CAParameter::GetName() const -> const char * {
	return mc_name;
}
auto CAParameter::GetDescription() const -> const char * {
	return mc_description;
}

}  // namespace parameters
