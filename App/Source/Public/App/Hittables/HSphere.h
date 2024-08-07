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
        HSphere(const DirectX::XMFLOAT3& InCenter, float InRadius, std::shared_ptr<Material> InMaterial, const HittableDepthType InDepthType, const float tmp_InOuterRefractionIndex)
            : Hittable(CLASS_HTYPE, InDepthType, tmp_InOuterRefractionIndex), m_center(InCenter), m_radius(fmaxf(0, InRadius)), m_material(InMaterial)
        {
        }
    };
}