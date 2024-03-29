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
    CAParametersTable(const CAParametersTable&) = delete;
    CAParametersTable(const CAParametersTable&&) = delete;
    auto operator=(const CAParametersTable&) -> CAParametersTable& = delete;
    auto operator=(CAParametersTable&&) -> CAParametersTable& = delete;

    // NOLINTNEXTLINE(readability-identifier-naming)
    [[nodiscard]] virtual auto begin() const noexcept -> CAParameter* const* = 0;
    // NOLINTNEXTLINE(readability-identifier-naming)
    [[nodiscard]] virtual auto end() const noexcept -> CAParameter* const* = 0;

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
    [[nodiscard]] CAParameter* const* begin() const noexcept final { return GetTableBegin(); }
    // NOLINTNEXTLINE(readability-identifier-naming)
    [[nodiscard]] CAParameter* const* end() const noexcept final { return GetTableEnd(); }

    [[nodiscard]] constexpr auto GetTableBegin() const noexcept { return mc_ptr_list.cbegin(); }

    [[nodiscard]] constexpr auto GetTableEnd() const noexcept { return mc_ptr_list.cend(); }
};

}  // namespace utility::parameters

#endif /* PARAMETER_TABLE_HPP */
