#include "App/Assets/Materials/MetalMat.h"
#include "App/Hittables/Hittable.h"
#include "App/Maths/RMathRand.h"

using namespace AppNmsp;
using namespace DirectX;



bool MetalMat::Scatter(const RayVECAnyNrm& InRayVec, const HitRecord& InRecord, XMFLOAT3& OutAttenuationColor, RayVECAnyNrm& OutRayScattered) const
{
    static thread_local LocalVectorDistributionUnitSphereDistribution distrib;

    XMVECTOR reflectedVec = XMVector3Reflect(InRayVec.Direction,  XMLoadFloat3(&InRecord.SurfaceNormal));

    OutRayScattered = RayVECAnyNrm
    {
        .Origin = XMLoadFloat3(&InRecord.ImpactPoint),
        .Direction = reflectedVec
    };
    OutAttenuationColor = m_albedo;
    return true;
}

#if WITH_REFERENCE
bool MetalMat::scatter(const ray& InRayVec, const hit_record& InRecord, color& OutAttenuation, ray& OutRayScattered) const
{
    vec3  reflected = reflect(InRecord.normal, InRecord.normal);
    OutRayScattered = ray(InRecord.p, reflected);
    OutAttenuation = m_albedo;
    return true;
}
#endif
