#pragma once

#include "App/Hittables/Hittable.h"

#include <memory>
#include <vector>

namespace AppNmsp
{
    class HList : public Hittable 
    {
    public:
        static constexpr HittableType CLASS_HTYPE = HittableType::HList;

    private:
        std::vector<std::shared_ptr<Hittable>> m_hittables;

    public:
        inline void Add(std::shared_ptr<Hittable> InHittable) { m_hittables.push_back(InHittable); }
        inline void Clear() { m_hittables.clear(); }

        virtual bool Hit(const RayVECAnyNrm& InRayVec, const float InRayTMin, const float InRayTMax, HitRecord& OutRecord) const R_PURE_OVERRIDE final;

    public:
        HList() : Hittable(CLASS_HTYPE) {}
        HList(std::shared_ptr<Hittable> InHittable) : Hittable(CLASS_HTYPE) { Add(InHittable); }
    };
};