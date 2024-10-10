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

        virtual bool Hit(const RayFLTAnyNrm& InRayVec, const FInterval InRayInterval, HitRecord& OutRecord) const R_PURE_OVERRIDE final;
#if WITH_REFERENCE
        virtual bool Hit(const ray& InRay, const DInterval InRayInterval, hit_record& OutRecord) const R_PURE_OVERRIDE final;
#endif

    public:
        HList() : Hittable(CLASS_HTYPE, HittableDepthType::Unknown, RefractionIndex::Air) {}
        HList(std::shared_ptr<Hittable> InHittable) : Hittable(CLASS_HTYPE, HittableDepthType::Unknown, RefractionIndex::Air) { Add(InHittable); }
    };
};