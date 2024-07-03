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
    class Material;
    class HitRecord {
    public:
        DirectX::XMFLOAT3 ImpactPoint;
        //The surface pointing against the incoming ray (outer or inner, based on 'bFrontFace' value)
        DirectX::XMFLOAT3 SurfaceNormal;
        //The "time" of ray, meaning how far the impact point is form the ray's origin
        float IncomingRayTValue;
        //Is the given normal pointing out of the object (true) or inside of it (false) ?
        bool bFrontFace;

        /// <summary>
        /// Set frontFace and normal parameter
        /// </summary>
        /// <param name="InRay">The ray, loaded into registeries</param>
        /// <param name="InOutwardNormalNrmlzd">The surface pointing out of surface</param>
        /// <returns></returns>
        inline void XM_CALLCONV SetFaceNormal(const RayVECAnyNrm& InRay, DirectX::FXMVECTOR InOutwardNormalNrmlzd)
        {
            bFrontFace = DirectX::XMVectorGetX(DirectX::XMVector3Dot(InRay.Direction, InOutwardNormalNrmlzd)) < 0;
            DirectX::XMStoreFloat3(&SurfaceNormal, bFrontFace ? InOutwardNormalNrmlzd : DirectX::XMVectorScale(InOutwardNormalNrmlzd, -1.f));
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
