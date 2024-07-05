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

   XMVECTOR refracted = XMVector3Refract(NormalizedDirection, XMLoadFloat3(&InRecord.SurfaceNormal), refractionIndex);
   OutRayScattered = RayVECAnyNrm
   {
       .Origin = XMLoadFloat3(&InRecord.ImpactPoint),
       .Direction = refracted
   };
    return true;
}

#if WITH_REFERENCE
bool DielectricMat::scatter(const ray& InRayVec, const hit_record& InRecord, color& OutAttenuation, ray& OutRayScattered) const
{
    OutAttenuation = color(1.0, 1.0, 1.0);
    double ri = InRecord.front_face ? (1.0 / m_refractionIndex) : m_refractionIndex;

    vec3 unit_direction = unit_vector(InRayVec.direction());
    vec3 refracted = refract(unit_direction, InRecord.normal, ri);

    OutRayScattered = ray(InRecord.p, refracted);
    return true;

}
#endif
