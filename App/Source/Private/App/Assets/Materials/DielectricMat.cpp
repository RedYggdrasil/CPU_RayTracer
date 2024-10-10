#include "App/Assets/Materials/DielectricMat.h"
#include "App/Hittables/Hittable.h"
#include "App/Maths/RMathRand.h"

using namespace AppNmsp;
using namespace DirectX;

#if USE_SCHLICK_REFLECTANCE
static float SchlickReflectance(const float InCosIncidentAngle, const float InRefractionIndex) {
    // Use Schlick's approximation for reflectance.
    float r0 = (1.f - InRefractionIndex) / (1.f + InRefractionIndex);
    r0 = r0 * r0;
    return r0 + (1.f - r0) * powf((1.f - InCosIncidentAngle), 5.f);
}
#endif

bool DielectricMat::Scatter(const RayFLTAnyNrm& InRayVec, const HitRecord& InRecord, XMFLOAT3& OutAttenuationColor, RayFLTAnyNrm& OutRayScattered) const
{

   OutAttenuationColor = FLOAT3_ONE;
   
   float refractionIndex = InRecord.bFrontFace ?
       InRecord.OuterRefractionIndex / m_refractionIndex
       : m_refractionIndex / InRecord.OuterRefractionIndex;

   XMVECTOR NormalizedDirection = XMVector3Normalize(XMLoadFloat3(&InRayVec.Direction));
   XMVECTOR SurfaceNormal = XMLoadFloat3(&InRecord.SurfaceNormal);

   float cosTheta = fminf(XMVectorGetX(XMVector3Dot(-NormalizedDirection, SurfaceNormal)), 1.f);


#if !USE_SCHLICK_REFLECTANCE
   XMVECTOR lCosTheta = XMVECTOR{ cosTheta ,0.f ,0.f ,0.f };
   XMVECTOR lRI = XMVECTOR{ refractionIndex ,0.f ,0.f ,0.f };
#endif

   XMVECTOR refracted = XMVector3Refract(NormalizedDirection, SurfaceNormal, refractionIndex);
   XMVECTOR resultDirection;

   static thread_local RRenderRandomizer::LocalRealDistribution<float> distrib = RRenderRandomizer::s_GetLocalDesyncDistribution<float>(0.f, 1.f);

   if (RMath::XMVector3EpsilonZero(refracted) || 
#if USE_SCHLICK_REFLECTANCE
       SchlickReflectance(cosTheta, refractionIndex)
#else
       XMVectorGetX(XMFresnelTerm(lCosTheta, lRI))
#endif
> distrib())
   {
       resultDirection = XMVector3Reflect(NormalizedDirection, SurfaceNormal);
   }
   else
   {
       resultDirection = refracted;
   }

   OutRayScattered.Origin = InRecord.ImpactPoint;
   XMStoreFloat3(&OutRayScattered.Direction, resultDirection);

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
