#pragma once

#include "App/pch.h"
#include "App/Maths/Ray.h"

namespace AppNmsp
{
    class HitRecord {
    public:
        DirectX::XMFLOAT3 p;
        DirectX::XMFLOAT3 normal;
        float t;
        bool frontFace;

        /// <summary>
        /// Set frontFace and normal parameter
        /// </summary>
        /// <param name="InRay">The ray, loaded into registeries</param>
        /// <param name="InOutwardNormalNrmlzd">The surface pointing out of surface</param>
        /// <returns></returns>
        inline void XM_CALLCONV SetFaceNormal(const RayVECAnyNrm& InRay, DirectX::FXMVECTOR InOutwardNormalNrmlzd)
        {
            frontFace = DirectX::XMVectorGetX(DirectX::XMVector3Dot(InRay.Direction, InOutwardNormalNrmlzd)) < 0;
            DirectX::XMStoreFloat3(&normal, frontFace ? InOutwardNormalNrmlzd : DirectX::XMVectorScale(InOutwardNormalNrmlzd, -1.f));
        }
    };

    //TODO : Reword this into a enum-union format
    class Hittable
    {
    public:
        virtual ~Hittable() = default;

        virtual bool Hit(const RayVECAnyNrm& InRayVec, const float InRayTMin, const float InRayTMax, HitRecord& OutRecord) const R_PURE;
    };
};
