#include "App/Assets/Materials/LambertianMat.h"
#include "App/Hittables/Hittable.h"
#include "App/Maths/RMathRand.h"

using namespace AppNmsp;
using namespace DirectX;



bool LambertianMat::Scatter(const RayFLTAnyNrm& InRayVec, const HitRecord& InRecord, XMFLOAT3& OutAttenuationColor, RayFLTAnyNrm& OutRayScattered) const
{
    static thread_local LocalVectorDistributionUnitSphereDistribution distrib;

    XMVECTOR scatterDirection = XMLoadFloat3(&InRecord.SurfaceNormal) + distrib.RandomUnitVector();
    
    if (RMath::XMVector3Small(scatterDirection))
    { scatterDirection = XMLoadFloat3(&InRecord.SurfaceNormal); }

    OutRayScattered.Origin = InRecord.ImpactPoint;
    XMStoreFloat3(&OutRayScattered.Direction, scatterDirection);
    OutAttenuationColor = m_albedo;

    return true;
}

#if WITH_REFERENCE
bool LambertianMat::scatter(const ray& InRayVec, const hit_record& InRecord, color& OutAttenuation, ray& OutRayScattered) const
{
    auto scatter_direction = InRecord.normal + random_unit_vector();
    OutRayScattered = ray(InRecord.p, scatter_direction);
    OutAttenuation = m_albedo;
    return true;
}
#endif
