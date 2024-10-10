#pragma once
#include "App/Assets/Materials/Material.h"

namespace AppNmsp
{
    /// <summary>
    /// Simplified Lambertian material than always scatter light
    /// </summary>
    class LambertianMat : public Material 
    {
    private:
        DirectX::XMFLOAT3 m_albedo;
    public:
        LambertianMat(const DirectX::XMFLOAT3& InAlbedo) : m_albedo(InAlbedo) {}

        virtual bool Scatter(const RayFLTAnyNrm& InRayVec, const HitRecord& InRecord, DirectX::XMFLOAT3& OutAttenuationColor, RayFLTAnyNrm& OutRayScattered) const override;
#if WITH_REFERENCE
        virtual bool scatter(const ray& InRayVec, const hit_record& InRecord, color& OutAttenuation, ray& OutRayScattered) const override;
#endif

    };
}