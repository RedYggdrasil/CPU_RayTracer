#include "App/Hittables/HList.h"

using namespace AppNmsp;
using namespace DirectX;

bool HList::Hit(const RayVECAnyNrm& InRayVec, const Interval InRayInterval, HitRecord& OutRecord) const
{
    HitRecord localRecord;
    bool bDidHit = false;
    float ClosestResult = InRayInterval.Max;

    for (const std::shared_ptr<Hittable>& hittable : m_hittables) 
    {
        if (hittable->Hit(InRayVec, Interval(InRayInterval.Min, ClosestResult), localRecord))
        {
            bDidHit = true;
            ClosestResult = localRecord.t;
            OutRecord = localRecord;
        }
    }

    return bDidHit;
}