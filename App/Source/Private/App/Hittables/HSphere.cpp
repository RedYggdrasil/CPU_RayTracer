#include "App/Hittables/HSphere.h"

using namespace AppNmsp;
using namespace DirectX;

bool HSphere::Hit(const RayVECAnyNrm& InRayVec, const Interval InRayInterval, HitRecord& OutRecord) const
{
    XMVECTOR lCenter = XMLoadFloat3(&m_center);
    XMVECTOR oc = lCenter - InRayVec.Origin;
    float a = XMVectorGetX(XMVector3LengthSq(InRayVec.Direction));
    float h = XMVectorGetX(XMVector3Dot(InRayVec.Direction, oc));
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

    OutRecord.t = root;
    XMVECTOR lHitLocation = InRayVec.At(OutRecord.t);
    XMStoreFloat3(&OutRecord.p, lHitLocation);

    XMVECTOR OutwardNormal = (lHitLocation - lCenter) / m_radius;
    OutRecord.SetFaceNormal(InRayVec, OutwardNormal);
    return true;
}