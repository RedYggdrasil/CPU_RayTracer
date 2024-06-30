#pragma once
#include "App/pch.h"
#include <algorithm>

namespace AppNmsp
{
    template <typename TReal>
    struct Interval final
    {
    public:
        TReal Min;
        TReal Max;
    public:
        inline constexpr TReal size() const {
            return Max - Min;
        }

        /// <summary>
        /// Value in inside the given range (include extremities) 
        /// </summary>
        /// <param name="InValue">Value to test</param>
        /// <returns>Is value inside the given range (including extremities) ?</returns>
        inline constexpr bool Contains(const TReal InValue) const {
            return Min <= InValue && InValue <= Max;
        }

        /// <summary>
        /// Value in inside the given range (exclude extremities) 
        /// </summary>
        /// <param name="InValue">Value to test</param>
        /// <returns>Is value inside the given range (excluding extremities) ?</returns>
        inline constexpr bool Surrounds(const TReal InValue) const {
            return Min < InValue && InValue < Max;
        }
        inline constexpr TReal Clamp(const TReal InValue) const { return std::clamp<TReal>(InValue, Min, Max); }
    public:

        constexpr Interval() : Min(+std::numeric_limits<TReal>::infinity()), Max(-std::numeric_limits<TReal>::infinity()) {}
        constexpr Interval(const TReal InMin, const TReal InMax) : Min(InMin), Max(InMax) {}
    };

    using FInterval = Interval<float>;
    using DInterval = Interval<double>;

    constexpr FInterval INTERVAL_EMPTY      = FInterval(+std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity());
    constexpr FInterval INTERVAL_UNIVERSE   = FInterval(-std::numeric_limits<float>::infinity(), +std::numeric_limits<float>::infinity());



}