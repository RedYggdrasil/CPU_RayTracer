#pragma once
#include "App/Assets/Materials/Material.h"

namespace AppNmsp
{
    /// <summary>
    /// metal material
    /// </summary>
    class MetalMat : public Material
    {
    private:
        DirectX::XMFLOAT3 m_albedo;
        float m_fuzziness;
    public:
        MetalMat(const DirectX::XMFLOAT3& InAlbedo, const float InFuzziness) : m_albedo(InAlbedo), m_fuzziness(InFuzziness) {}

        virtual bool Scatter(const RayVECAnyNrm& InRayVec, const HitRecord& InRecord, DirectX::XMFLOAT3& OutAttenuationColor, RayVECAnyNrm& OutRayScattered) const override;
#if WITH_REFERENCE
        virtual bool scatter(const ray& InRayVec, const hit_record& InRecord, color& OutAttenuation, ray& OutRayScattered) const override;
#endif

    };
}