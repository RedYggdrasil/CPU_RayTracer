#pragma once

#include "App/Assets/Materials/Material.h"

namespace AppNmsp
{
    class DielectricMat : public Material
    {
    private:
        /*Self refraction index, regardless of outer environment*/
        float m_refractionIndex;
    public:
        DielectricMat(const float InRefractionIndex) : m_refractionIndex(InRefractionIndex) {}

        virtual bool Scatter(const RayFLTAnyNrm& InRayVec, const HitRecord& InRecord, DirectX::XMFLOAT3& OutAttenuationColor, RayFLTAnyNrm& OutRayScattered) const override;
#if WITH_REFERENCE
        virtual bool scatter(const ray& InRayVec, const hit_record& InRecord, color& OutAttenuation, ray& OutRayScattered) const override;
#endif

    };
}