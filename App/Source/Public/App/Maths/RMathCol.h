#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "App/pch.h"
#include "App/Maths/Ray.h"

namespace AppNmsp
{
	class RMathCol
	{
    public:
        inline static float XM_CALLCONV SphereCollisionOnRay(const RayVECAnyNrm* InRayVec, DirectX::FXMVECTOR InSphereCenter, const float InRadius)
        {
            DirectX::XMVECTOR RayOriginToSphereCenter = DirectX::XMVectorSubtract(InSphereCenter, InRayVec->Origin);
            const float a = DirectX::XMVectorGetX(DirectX::XMVector3Dot(InRayVec->Direction, InRayVec->Direction));
            const float b = -2.f * DirectX::XMVectorGetX(DirectX::XMVector3Dot(InRayVec->Direction, RayOriginToSphereCenter));
            const float c = DirectX::XMVectorGetX(DirectX::XMVector3Dot(RayOriginToSphereCenter, RayOriginToSphereCenter)) - (InRadius * InRadius);
            const float discriminant = b * b - 4.f * a * c;
            if (discriminant < 0) {
                return -1.0f;
            }
            else {
                return (-b - sqrtf(discriminant)) / (2.f * a);
            }
        }
		inline static bool XM_CALLCONV RaySphereCollide(const RayVECAnyNrm* InRayVec, DirectX::FXMVECTOR InSphereCenter, const float InRadius)
		{
            return SphereCollisionOnRay(InRayVec, InSphereCenter, InRadius) >= 0.f;
        }
	};
};