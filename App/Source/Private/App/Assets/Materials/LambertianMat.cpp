#include "App/Assets/Materials/LambertianMat.h"
#include "App/Hittables/Hittable.h"
#include "App/Maths/RMathRand.h"

using namespace AppNmsp;
using namespace DirectX;



bool LambertianMat::Scatter(const RayVECAnyNrm& InRayVec, const HitRecord& InRecord, XMFLOAT3& OutAttenuationColor, RayVECAnyNrm& OutRayScattered) const
{
    static thread_local LocalVectorDistributionUnitSphereDistribution distrib;

    XMVECTOR scatterDirection = XMLoadFloat3(&InRecord.SurfaceNormal) + distrib.RandomUnitVector();
    
    if (RMath::XMVector3Small(scatterDirection))
    { scatterDirection = XMLoadFloat3(&InRecord.SurfaceNormal); }

    OutRayScattered = RayVECAnyNrm
    {
        .Origin = XMLoadFloat3(&InRecord.ImpactPoint),
        .Direction = scatterDirection
    };
    OutAttenuationColor = m_albedo;
    return true;
}

#if WITH_REFERENCE
bool LambertianMat::Scatter(const ray& InRayVec, const hit_record& InRecord, color& OutAttenuation, ray& OutRayScattered) const
{
    auto scatter_direction = InRecord.normal + random_unit_vector();
    OutRayScattered = ray(rec.p, scatter_direction);
    OutAttenuationColor = m_albedo;
    return true;
}
#endif
