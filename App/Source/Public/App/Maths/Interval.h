#pragma once
#include "App/pch.h"
#include <algorithm>

namespace AppNmsp
{
    struct Interval final
    {
    public:
        float Min;
        float Max;
    public:
        inline constexpr float size() const {
            return Max - Min;
        }

        /// <summary>
        /// Value in inside the given range (include extremities) 
        /// </summary>
        /// <param name="InValue">Value to test</param>
        /// <returns>Is value inside the given range (including extremities) ?</returns>
        inline constexpr bool Contains(const float InValue) const {
            return Min <= InValue && InValue <= Max;
        }

        /// <summary>
        /// Value in inside the given range (exclude extremities) 
        /// </summary>
        /// <param name="InValue">Value to test</param>
        /// <returns>Is value inside the given range (excluding extremities) ?</returns>
        inline constexpr bool Surrounds(const float InValue) const {
            return Min < InValue && InValue < Max;
        }
        inline constexpr float Clamp(const float InValue) const { return std::clamp<float>(InValue, Min, Max); }
    public:

        constexpr Interval() : Min(+R_INFINITY_F), Max(-R_INFINITY_F) {}
        constexpr Interval(const float InMin, const float InMax) : Min(InMin), Max(InMax) {}
    };

    constexpr Interval INTERVAL_EMPTY = Interval(+R_INFINITY_F, -R_INFINITY_F);
    constexpr Interval INTERVAL_UNIVERSE = Interval(-R_INFINITY_F, +R_INFINITY_F);
}