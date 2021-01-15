/*
 * ParamertesBaseHelper.hpp
 *
 *  Created on: 26.11.2020
 *      Author: sebastian
 */

#ifndef PARAMETERSBASEHELPER_HPP
#define PARAMETERSBASEHELPER_HPP

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define INIT_PARAMETER(parameterType, name, converter, val, desc)                    \
    template <>                                                                      \
    decltype(parameterType::m_instance) parameterType::m_instance{                   \
        name, desc, val,                                                             \
        converter}; /*NOLINT(cppcoreguidelines-avoid-non-const-global-variables)*/   \
    template <>                                                                      \
    decltype(parameterType::mc_static_ptr) parameterType::                           \
        mc_static_ptr /*NOLINT(cppcoreguidelines-avoid-non-const-global-variables)*/ \
            __USED PLACE_IN_SECTION(                                                 \
                ".parameters_tbl") /*NOLINT(clang-diagnostic-ignored-attributes) */  \
        = &parameterType::m_instance;                                                \
    template <>                                                                      \
    auto parameterType::GetInstance() noexcept->parameterType& {                     \
        return *mc_static_ptr; /* NOLINT(clang-diagnostic-undefined-var-template) */ \
    }

#endif /* PARAMETERSBASEHELPER_HPP */
