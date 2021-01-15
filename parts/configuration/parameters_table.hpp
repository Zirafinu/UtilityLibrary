/*
 * parameter_table.hpp
 *
 *  Created on: 12.01.2021
 *      Author: sebastian
 */

#ifndef PARAMETER_TABLE_HPP
#define PARAMETER_TABLE_HPP

#include <array>
#include <tuple>

namespace utility::parameters {

class CAParameter;

class CAParametersTable {
public:
	virtual ~CAParametersTable() = default;

	// NOLINTNEXTLINE(readability-identifier-naming)
	[[nodiscard]] virtual auto begin() const noexcept-> CAParameter*const* = 0;
	// NOLINTNEXTLINE(readability-identifier-naming)
	[[nodiscard]] virtual auto end() const noexcept -> CAParameter*const* = 0;

	[[nodiscard]] static auto GetInstance() noexcept -> CAParametersTable&;
protected:
	CAParametersTable() = default;
};

template <typename... PARAMETERS>
class CCParametersTableImpl : public CAParametersTable {
  const std::array<CAParameter*, std::tuple_size<std::tuple<PARAMETERS...>>::value> mc_ptr_list{
      &PARAMETERS::GetInstance()...};

 public:
  CCParametersTableImpl() : CAParametersTable{} {};

	// NOLINTNEXTLINE(readability-identifier-naming)
  [[nodiscard]] CAParameter* const* begin() const noexcept final { return GetFirst(); }
	// NOLINTNEXTLINE(readability-identifier-naming)
  [[nodiscard]] CAParameter* const* end() const noexcept final { return GetLast(); }

  [[nodiscard]] constexpr auto GetFirst() const noexcept -> CAParameter* const* {
    return mc_ptr_list.cbegin();
  }

  [[nodiscard]] constexpr auto GetLast() const noexcept -> CAParameter* const* {
    return mc_ptr_list.cend();
  }
};

}  // namespace utility::parameters


#endif /* PARAMETER_TABLE_HPP */
