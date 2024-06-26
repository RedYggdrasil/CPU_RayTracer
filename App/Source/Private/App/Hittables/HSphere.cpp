#include "App/Hittables/HSphere.h"

using namespace AppNmsp;
using namespace DirectX;

bool HSphere::Hit(const RayVECAnyNrm& InRayVec, const float InRayTMin, const float InRayTMax, HitRecord& OutRecord) const
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

    float sqrtd = sqrtf(discriminant);

    // Find the nearest root that lies in the acceptable range.
    float root = (h - sqrtd) / a;
    if (root <= InRayTMin || InRayTMax <= root) 
    {
        root = (h + sqrtd) / a;
        if (root <= InRayTMin || InRayTMax <= root)
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