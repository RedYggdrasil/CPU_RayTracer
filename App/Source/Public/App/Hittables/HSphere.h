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
    public:
        virtual bool Hit(const RayVECAnyNrm& InRayVec, const float InRayTMin, const float InRayTMax, HitRecord& OutRecord) const R_PURE_OVERRIDE final;
    public:
        HSphere(const DirectX::XMFLOAT3& InCenter, float InRadius) : Hittable(CLASS_HTYPE), m_center(InCenter), m_radius(fmaxf(0, InRadius)) {}
    };
}