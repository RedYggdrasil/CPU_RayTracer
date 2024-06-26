#pragma once

#include "App/Hittables/Hittable.h"

namespace AppNmsp
{
    class HSphere : public Hittable {
    private:
        DirectX::XMFLOAT3 m_center;
        float m_radius;
    public:
        HSphere(const DirectX::XMFLOAT3& InCenter, float InRadius) : m_center(InCenter), m_radius(fmaxf(0, InRadius)) {}

        virtual bool Hit(const RayVECAnyNrm& InRayVec, const float InRayTMin, const float InRayTMax, HitRecord& OutRecord) const R_PURE_OVERRIDE;
    };
}