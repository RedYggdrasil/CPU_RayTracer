#include "App/Assets/Materials/DielectricMat.h"
#include "App/Hittables/Hittable.h"
#include "App/Maths/RMathRand.h"

using namespace AppNmsp;
using namespace DirectX;


bool DielectricMat::Scatter(const RayVECAnyNrm& InRayVec, const HitRecord& InRecord, XMFLOAT3& OutAttenuationColor, RayVECAnyNrm& OutRayScattered) const
{
   OutAttenuationColor = FLOAT3_ONE;
   
   float refractionIndex = InRecord.bFrontFace ?
       InRecord.OuterRefractionIndex / m_refractionIndex
       : m_refractionIndex / InRecord.OuterRefractionIndex;

   XMVECTOR NormalizedDirection = XMVector3Normalize(InRayVec.Direction);

   XMVECTOR SurfaceNormal = XMLoadFloat3(&InRecord.SurfaceNormal);
   XMVECTOR refracted = XMVector3Refract(NormalizedDirection, SurfaceNormal, refractionIndex);
   XMVECTOR resultDirection;
   if (RMath::XMVector3EpsilonZero(refracted))
   {
       resultDirection = XMVector3Reflect(NormalizedDirection, SurfaceNormal);
   }
   else
   {
       resultDirection = refracted;
   }

   OutRayScattered = RayVECAnyNrm
   {
       .Origin = XMLoadFloat3(&InRecord.ImpactPoint),
       .Direction = resultDirection
   };
    return true;
}

#if WITH_REFERENCE
bool DielectricMat::scatter(const ray& InRayVec, const hit_record& InRecord, color& OutAttenuation, ray& OutRayScattered) const
{
    OutAttenuation = color(1.0, 1.0, 1.0);
    double ri = InRecord.front_face ? (1.0 / m_refractionIndex) : m_refractionIndex;

    vec3 unit_direction = unit_vector(InRayVec.direction());

    double cos_theta = fmin(dot(-unit_direction, InRecord.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = ri * sin_theta > 1.0;
    vec3 direction;

    if (cannot_refract)
        direction = reflect(unit_direction, InRecord.normal);
    else
        direction = refract(unit_direction, InRecord.normal, ri);

    OutRayScattered = ray(InRecord.p, direction);
    return true;

}
#endif
