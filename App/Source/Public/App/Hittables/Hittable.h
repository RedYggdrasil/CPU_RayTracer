#pragma once

#include "App/pch.h"
#include "App/Defines/RefractionIndexes.h"
#include "App/Maths/Ray.h"
#include "App/Maths/Interval.h"

enum class HittableType : uint8_t
{
    RENUM_MIN_VAL(Unknown, 0),
    HSphere,
    RENUM_MAX(HList)
};
RS_DEFINE_ENUM(HittableType);

/// <summary>
/// This enum represent the "depth" priority of any hittable, at point x,y,z in space, 
/// if we are inside the world, an ocean, and an air bubble, is the world inside the air buble ? or the other way around ?
/// this depthType represent a sort of priority list of that where HittableDepthType::Max is higher priority
/// </summary>
enum class HittableDepthType : uint8_t
{
    RENUM_MIN_VAL(Unknown, 0),
    World0,
    WorldLiquid0,
    Container0,
    Container1,
    Container2,
    RENUM_MAX(Solid3)
};
RS_DEFINE_ENUM(HittableDepthType);


namespace AppNmsp
{
    class Material;
    class HitRecord 
    {
    public:
        DirectX::XMFLOAT3 ImpactPoint;
        //The surface pointing against the incoming ray (outer or inner, based on 'bFrontFace' value)
        DirectX::XMFLOAT3 SurfaceNormal;
        std::shared_ptr<Material> SurfaceMaterial;
        //The "time" of ray, meaning how far the impact point is form the ray's origin
        float IncomingRayTValue;
        //Is the given normal pointing out of the object (true) or inside of it (false) ?
        bool bFrontFace;
        //The refraction index of the outer environment (bFrontFace ? RayInEnv : RayOutEnv)
        float OuterRefractionIndex;
        /// <summary>
        /// Set frontFace and normal parameter
        /// </summary>
        /// <param name="InRay">The ray's direction, loaded into registery</param>
        /// <param name="InOutwardNormalNrmlzd">The normal pointing out of surface</param>
        /// <returns></returns>
        inline void XM_CALLCONV SetFaceNormal(DirectX::FXMVECTOR InRayDirection, DirectX::FXMVECTOR InOutwardNormalNrmlzd)
        {
            bFrontFace = DirectX::XMVectorGetX(DirectX::XMVector3Dot(InRayDirection, InOutwardNormalNrmlzd)) <= 0.f;
            DirectX::XMStoreFloat3(&SurfaceNormal, bFrontFace ? InOutwardNormalNrmlzd : DirectX::XMVectorScale(InOutwardNormalNrmlzd, -1.f));
        }
    };

#if WITH_REFERENCE
    class hit_record {
    public:
        point3 p;
        vec3 normal;
        std::shared_ptr<Material> mat;
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
    protected:
        inline static size_t TotalInstanceCount = std::numeric_limits<size_t>::max(); /*(First Preincrement loop back to 0)*/
    public:
        const HittableType HType;
        const HittableDepthType HDepthType;
        /*This is a temp value, in the future we will need to compute a list of overlapping items and
        use that list to return a refraction index based on a exit point in space */
        const float tmp_OuterRefractionIndex;
        const size_t InstanceID;
        //Allow sorting by type
        inline bool operator<(const Hittable& InOther) const 
        { return HType < InOther.HType; }
    public:

        virtual bool GetIsRefractable() const { return false; }
        virtual float GetInnerRefractionIndex() const { return 0.0; }

        /*TODO : Generate a list of overllaped collider associated with EnvironmentData and return refractance based on position*/
        inline float GetOuterRefractionIndex(const DirectX::XMFLOAT3& InPosition) const { return tmp_OuterRefractionIndex; }
        /*TODO : Generate a list of overllaped collider associated with EnvironmentData and return refractance based on position*/
        inline float XM_CALLCONV GetOuterRefractionIndex(DirectX::FXMVECTOR InPosition) const { return tmp_OuterRefractionIndex; }

        virtual bool Hit(const RayFLTAnyNrm& InRayVec, const FInterval InRayInterval, HitRecord& OutRecord) const R_PURE;
#if WITH_REFERENCE
        virtual bool Hit(const ray& InRay, const DInterval InRayInterval, hit_record& OutRecord) const R_PURE;
#endif

    protected:
        Hittable(const HittableType InHType, const HittableDepthType InDepthType, const float tmp_InOuterRefractionIndex) : HType(InHType), HDepthType(InDepthType), tmp_OuterRefractionIndex(tmp_InOuterRefractionIndex), InstanceID(++TotalInstanceCount) {};
    public:
        virtual ~Hittable() = default;
    };
};
