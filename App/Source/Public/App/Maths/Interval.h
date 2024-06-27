#pragma once
#include "App/pch.h"

namespace AppNmsp
{
    struct Interval final
    {
    public:
        float Min;
        float Max;
    public:
        float size() const {
            return Max - Min;
        }

        /// <summary>
        /// Value in inside the given range (include extremities) 
        /// </summary>
        /// <param name="InValue">Value to test</param>
        /// <returns>Is value inside the given range (including extremities) ?</returns>
        bool Contains(const float InValue) const {
            return Min <= InValue && InValue <= Max;
        }

        /// <summary>
        /// Value in inside the given range (exclude extremities) 
        /// </summary>
        /// <param name="InValue">Value to test</param>
        /// <returns>Is value inside the given range (excluding extremities) ?</returns>
        bool Surrounds(const float InValue) const {
            return Min < InValue && InValue < Max;
        }
    public:

        constexpr Interval() : Min(+R_INFINITY_F), Max(-R_INFINITY_F) {}
        constexpr Interval(const float InMin, const float InMax) : Min(InMin), Max(InMax) {}
    };

    constexpr Interval INTERVAL_EMPTY = Interval(+R_INFINITY_F, -R_INFINITY_F);
    constexpr Interval INTERVAL_UNIVERSE = Interval(-R_INFINITY_F, +R_INFINITY_F);
}