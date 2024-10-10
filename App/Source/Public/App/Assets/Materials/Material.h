#pragma once
#include "App/pch.h"
#include "App/Maths/Ray.h"

#if WITH_REFERENCE
class ray;
#endif
namespace AppNmsp
{
    class HitRecord;
#if WITH_REFERENCE
    class hit_record;
#endif
    class Material
    {
    public:
        virtual bool Scatter(const RayFLTAnyNrm& InRayVec, const HitRecord& InRecord, DirectX::XMFLOAT3& OutAttenuationColor, RayFLTAnyNrm& OutRayScattered) const {
            return false;
        }
#if WITH_REFERENCE
        virtual bool scatter(const ray& InRayVec, const hit_record& InRecord, color& OutAttenuation, ray& OutRayScattered) const
        {
            return false;
        }
#endif
    public:
        virtual ~Material() = default;
    };
};