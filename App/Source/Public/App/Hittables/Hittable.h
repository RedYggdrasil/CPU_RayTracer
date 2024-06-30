#pragma once

#include "App/pch.h"
#include "App/Maths/Ray.h"
#include "App/Maths/Interval.h"

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

#if WITH_REFERENCE
    class hit_record {
    public:
        point3 p;
        vec3 normal;
        double t;
        bool front_face;

        void set_face_normal(const ray& r, const vec3& outward_normal) {
            // Sets the hit record normal vector.
            // NOTE: the parameter `outward_normal` is assumed to have unit length.

            front_face = dot(r.direction(), outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }
    };
#endif

    //TODO : Reword this into a enum-union format
    class Hittable
    {
    public:
        const HittableType HType;
        //Allow sorting by type
        inline bool operator<(const Hittable& InOther) const 
        { return HType < InOther.HType; }
    public:
        virtual bool Hit(const RayVECAnyNrm& InRayVec, const FInterval InRayInterval, HitRecord& OutRecord) const R_PURE;
#if WITH_REFERENCE
        virtual bool Hit(const ray& InRay, const DInterval InRayInterval, hit_record& OutRecord) const R_PURE;
#endif

    protected:
        Hittable(const HittableType InHType) : HType(InHType) {};
    public:
        virtual ~Hittable() = default;
    };
};
