/*
 * Parameters.hpp
 *
 *  Created on: 26.11.2020
 *      Author: sebastian
 */

#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include <cstdint>
#include <gsl/pointers>
#include <gsl/span>

namespace utility::parameters {
template <typename T>
using NotNull = gsl::not_null<T *>;

class CAParameter {
   public:
    using UsedCharacters = std::size_t;
    using OutputStringBuffer = gsl::span<char, gsl::dynamic_extent>;
    using ValueString = gsl::span<const char, gsl::dynamic_extent>;
    using ErrorMessage = gsl::span<const char, gsl::dynamic_extent>;
    using ValueBytes = gsl::span<uint8_t, gsl::dynamic_extent>;
    using ConstValueBytes = gsl::span<const uint8_t, gsl::dynamic_extent>;

    static constexpr ErrorMessage NO_ERROR{nullptr, std::size_t(0)};

    virtual ~CAParameter() = default;
    CAParameter(const CAParameter &) = delete;
    CAParameter(CAParameter &&) = delete;
    auto operator=(const CAParameter &) -> CAParameter & = delete;
    auto operator=(CAParameter &&) -> CAParameter & = delete;

    /* accessors to local fields */
    [[nodiscard]] auto GetID() const -> uint16_t;
    [[nodiscard]] auto GetName() const -> const char *;
    [[nodiscard]] auto GetDescription() const -> const char *;

    [[nodiscard]] virtual auto ConvertValueToString(OutputStringBuffer string) const
        -> UsedCharacters = 0;

    [[nodiscard]] virtual auto ParseValueFromString(const ValueString &string) -> ErrorMessage = 0;

    [[nodiscard]] virtual auto GetValueBytes() -> ValueBytes = 0;
    [[nodiscard]] virtual auto GetValueBytes() const -> ConstValueBytes = 0;
    [[nodiscard]] virtual auto GetDefaultValueBytes() const -> ConstValueBytes = 0;

    /**
     *	Reset the value to the default value.
     */
    virtual void ResetValue() = 0;

   protected:
    CAParameter(const char *name, const char *description, uint16_t id)
        : mc_name(name), mc_description(description), mc_id(id) {}

   private:
    const char *const mc_name;
    const char *const mc_description;
    const uint16_t mc_id;
};

template <typename T>
class CAValueConverter {
   public:
    CAValueConverter() = default;
    virtual ~CAValueConverter() = default;

    [[nodiscard]] virtual auto Parse(const CAParameter::ValueString &, NotNull<T> value) const
        -> CAParameter::ErrorMessage = 0;

    [[nodiscard]] virtual auto Stringify(CAParameter::OutputStringBuffer, const T &value) const
        -> CAParameter::UsedCharacters = 0;

    // deleted functions
    CAValueConverter(const CAValueConverter &) = delete;
    CAValueConverter(CAValueConverter &&) = delete;
    auto operator=(const CAValueConverter &) -> CAValueConverter & = delete;
    auto operator=(CAValueConverter &&) -> CAValueConverter & = delete;
};

template <uint16_t ID, typename TYPE>
class CPlainParameter : public CAParameter {
    using ThisType = CPlainParameter<ID, TYPE>;

   public:
    ~CPlainParameter() override = default;

    CPlainParameter() = delete;
    CPlainParameter(const ThisType &) = delete;
    CPlainParameter(ThisType &&) = delete;
    auto operator=(const ThisType &) -> ThisType & = delete;
    auto operator=(ThisType &&) -> ThisType & = delete;

    [[nodiscard]] static auto GetInstance() noexcept -> ThisType &;

    [[nodiscard]] static auto Value() noexcept -> const TYPE & { return GetInstance().m_value; }

    [[nodiscard]] auto ConvertValueToString(OutputStringBuffer string) const
        -> UsedCharacters override {
        return m_converter.Stringify(string, m_value);
    }

    [[nodiscard]] auto ParseValueFromString(const ValueString &string) -> ErrorMessage override {
        return m_converter.Parse(string, NotNull<TYPE>(&m_value));
    }

    void ResetValue() override { m_value = mc_default_value; }

    [[nodiscard]] auto GetValue() const -> const TYPE & { return m_value; }

    [[nodiscard]] auto GetValue() -> TYPE & { return m_value; }

    [[nodiscard]] auto GetValueBytes() -> ValueBytes override {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        return {reinterpret_cast<uint8_t *>(&m_value), std::size_t(sizeof(TYPE))};
    }

    [[nodiscard]] auto GetValueBytes() const -> ConstValueBytes override {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        return {reinterpret_cast<const uint8_t *>(&m_value), std::size_t(sizeof(TYPE))};
    }

    [[nodiscard]] auto GetDefaultValueBytes() const -> ConstValueBytes override {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        return {reinterpret_cast<const uint8_t *>(&mc_default_value), std::size_t(sizeof(TYPE))};
    }

   private:
    TYPE m_value;
    const TYPE mc_default_value;
    const CAValueConverter<TYPE> &m_converter;

   protected:
    explicit CPlainParameter(const char *name, const char *description, const TYPE &default_value,
                             const CAValueConverter<TYPE> &pc_converter) noexcept
        : CAParameter(name, description, ID),
          m_value(default_value),
          mc_default_value(default_value),
          m_converter{pc_converter} {}
};

}  // namespace utility::parameters

#endif /* PARAMETERS_HPP */
