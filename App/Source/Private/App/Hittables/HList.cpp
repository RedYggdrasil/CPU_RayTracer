#include "App/Hittables/HList.h"

using namespace AppNmsp;
using namespace DirectX;

bool HList::Hit(const RayFLTAnyNrm& InRayVec, const FInterval InRayInterval, HitRecord& OutRecord) const
{
    HitRecord localRecord;
    bool bDidHit = false;
    float ClosestResult = InRayInterval.Max;

    for (const std::shared_ptr<Hittable>& hittable : m_hittables) 
    {
        if (hittable->Hit(InRayVec, FInterval(InRayInterval.Min, ClosestResult), localRecord))
        {
            bDidHit = true;
            ClosestResult = localRecord.IncomingRayTValue;
            OutRecord = localRecord;
        }
    }

    return bDidHit;
}

#if WITH_REFERENCE
bool HList::Hit(const ray& InRay, const DInterval InRayInterval, hit_record& OutRecord) const
{
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = InRayInterval.Max;

    for (const auto& object : m_hittables) {
        if (object->Hit(InRay, DInterval(InRayInterval.Min, closest_so_far), temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            OutRecord = temp_rec;
        }
    }

    return hit_anything;
}
#endif