#include "App/Hittables/HList.h"

using namespace AppNmsp;
using namespace DirectX;

bool HList::Hit(const RayVECAnyNrm& InRayVec, const float InRayTMin, const float InRayTMax, HitRecord& OutRecord) const
{
    HitRecord localRecord;
    bool bDidHit = false;
    float ClosestResult = InRayTMax;

    for (const std::shared_ptr<Hittable>& hittable : m_hittables) 
    {
        if (hittable->Hit(InRayVec, InRayTMin, ClosestResult, localRecord)) 
        {
            bDidHit = true;
            ClosestResult = localRecord.t;
            OutRecord = localRecord;
        }
    }

    return bDidHit;
}