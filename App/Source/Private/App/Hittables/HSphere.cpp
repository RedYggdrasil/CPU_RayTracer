#include "App/Hittables/HSphere.h"

using namespace AppNmsp;
using namespace DirectX;

bool HSphere::Hit(const RayFLTAnyNrm& InRayFLT, const FInterval InRayInterval, HitRecord& OutRecord) const
{
    XMVECTOR lCenter = XMLoadFloat3(&m_center);
    XMVECTOR lRayOrigin = XMLoadFloat3(&InRayFLT.Origin);
    XMVECTOR lRayDir = XMLoadFloat3(&InRayFLT.Direction);

    XMVECTOR oc = lCenter - lRayOrigin;
    float a = XMVectorGetX(XMVector3LengthSq(lRayDir));
    float h = XMVectorGetX(XMVector3Dot(lRayDir, oc));
    float c = XMVectorGetX(XMVector3LengthSq(oc)) - m_radius * m_radius;

    float discriminant = h * h - a * c;
    if (discriminant < 0.f)
    {
        return false;
    }

    float sqrtd = R_SQRTF(discriminant);

    // Find the nearest root that lies in the acceptable range.
    float root = (h - sqrtd) / a;
    if (!InRayInterval.Surrounds(root))
    {
        root = (h + sqrtd) / a;
        if (!InRayInterval.Surrounds(root))
        {
            return false;
        }
    }

    OutRecord.IncomingRayTValue = root;
    XMVECTOR lHitLocation = LRayVEC::At(lRayOrigin, lRayDir, OutRecord.IncomingRayTValue);

    XMVECTOR OutwardNormal = (lHitLocation - lCenter) / m_radius;

    OutRecord.SetFaceNormal(lRayDir, OutwardNormal);
    OutRecord.SurfaceMaterial = m_material;

    //Hit location is offseted to avoid float imprecision causing ray origin to be under the surface and imediatly rebouncing on it, causing weird circular artifacts.
    lHitLocation = lHitLocation
#if OBJECT_RAY_HIT_OUTWARD
        +(OutwardNormal * OBJECT_RAY_HIT_AMOUNT * (OutRecord.bFrontFace ? 1.f : -1.f))
#endif
        ;

    XMStoreFloat3(&OutRecord.ImpactPoint, lHitLocation);
    OutRecord.OuterRefractionIndex = this->GetOuterRefractionIndex(lHitLocation);
    return true;
}

#if WITH_REFERENCE
bool HSphere::Hit(const ray& InRay, const DInterval InRayInterval, hit_record& OutRecord) const
{
    vec3 oc = vec3(m_center) - InRay.origin();
    auto a = InRay.direction().length_squared();
    auto h = dot(InRay.direction(), oc);
    auto c = oc.length_squared() - (double)m_radius * (double)m_radius;

    auto discriminant = h * h - a * c;
    if (discriminant < 0)
        return false;

    auto sqrtd = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (h - sqrtd) / a;
    if (!InRayInterval.Surrounds(root)) {
        root = (h + sqrtd) / a;
        if (!InRayInterval.Surrounds(root))
            return false;
    }

    OutRecord.t = root;
    OutRecord.p = InRay.at(OutRecord.t);
    vec3 outward_normal = (OutRecord.p - vec3(m_center)) / (double)m_radius;
    OutRecord.set_face_normal(InRay, outward_normal);
    OutRecord.mat = m_material;
    return true;
}
#endif