#pragma once

#include "App/pch.h"
#include "App/Maths/Ray.h"

enum class HittableType : uint8_t
{
    RENUM_MIN_VAL(Unknown, 0),
    HSphere,
    RENUM_MAX(HList)
};
RS_DEFINE_ENUM(HittableType);

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
        const HittableType HType;
        //Allow sorting by type
        inline bool operator<(const Hittable& InOther) const 
        { return HType < InOther.HType; }
    public:
        virtual bool Hit(const RayVECAnyNrm& InRayVec, const float InRayTMin, const float InRayTMax, HitRecord& OutRecord) const R_PURE;
    
    protected:
        Hittable(const HittableType InHType) : HType(InHType) {};
    public:
        virtual ~Hittable() = default;
    };
};
