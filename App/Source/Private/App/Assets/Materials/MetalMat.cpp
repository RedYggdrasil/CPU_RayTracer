#include "App/Assets/Materials/MetalMat.h"
#include "App/Hittables/Hittable.h"
#include "App/Maths/RMathRand.h"

using namespace AppNmsp;
using namespace DirectX;



bool MetalMat::Scatter(const RayFLTAnyNrm& InRayVec, const HitRecord& InRecord, XMFLOAT3& OutAttenuationColor, RayFLTAnyNrm& OutRayScattered) const
{
    static thread_local LocalVectorDistributionUnitSphereDistribution distrib;

    XMVECTOR recordSurfaceNormal = XMLoadFloat3(&InRecord.SurfaceNormal);
    XMVECTOR lDirection = XMLoadFloat3(&InRayVec.Direction);
    XMVECTOR reflectedVec = XMVector3Reflect(lDirection, recordSurfaceNormal);
    reflectedVec = XMVector3Normalize(reflectedVec) + (distrib.RandomUnitVector() * m_fuzziness);
    
    OutRayScattered.Origin = InRecord.ImpactPoint;
    XMStoreFloat3(&OutRayScattered.Direction, reflectedVec);

    OutAttenuationColor = m_albedo;
    //Did the ray indeed point away from the surface ?
    return XMVectorGetX(XMVector3Dot(reflectedVec, recordSurfaceNormal)) > 0.f;
}

#if WITH_REFERENCE
bool MetalMat::scatter(const ray& InRayVec, const hit_record& InRecord, color& OutAttenuation, ray& OutRayScattered) const
{
    vec3  reflected = reflect(InRecord.normal, InRecord.normal);
    reflected = unit_vector(reflected) + (m_fuzziness * random_unit_vector());
    OutRayScattered = ray(InRecord.p, reflected);
    OutAttenuation = m_albedo;
    //Did the ray indeed point away from the surface ?
    return dot(OutRayScattered.direction(), InRecord.normal) > 0;
}
#endif
