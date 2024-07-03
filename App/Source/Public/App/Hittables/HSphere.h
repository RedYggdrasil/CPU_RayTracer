#pragma once

#include "App/Hittables/Hittable.h"

namespace AppNmsp
{
    class HSphere : public Hittable 
    {
    public:
        static constexpr HittableType CLASS_HTYPE = HittableType::HSphere;
    private:
        DirectX::XMFLOAT3 m_center;
        float m_radius;
        std::shared_ptr<Material> m_material;
    public:
        virtual bool Hit(const RayVECAnyNrm& InRayVec, const FInterval InRayInterval, HitRecord& OutRecord) const R_PURE_OVERRIDE final;
#if WITH_REFERENCE
        virtual bool Hit(const ray& InRay, const DInterval InRayInterval, hit_record& OutRecord) const R_PURE_OVERRIDE final;
#endif
    public:
        HSphere(const DirectX::XMFLOAT3& InCenter, float InRadius) : Hittable(CLASS_HTYPE), m_center(InCenter), m_radius(fmaxf(0, InRadius)) 
        {
            // TODO: Initialize the material pointer `mat`.
        }
    };
}