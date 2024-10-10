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
        
        inline static float XM_CALLCONV SphereCollisionOnRay(DirectX::FXMVECTOR InRayOrigin, DirectX::FXMVECTOR InRayDirection, DirectX::FXMVECTOR InSphereCenter, const float InRadius)
        {
            DirectX::XMVECTOR RayOriginToSphereCenter = DirectX::XMVectorSubtract(InSphereCenter, InRayOrigin);

            const float a = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(InRayDirection));

            const float h = DirectX::XMVectorGetX(DirectX::XMVector3Dot(InRayDirection, RayOriginToSphereCenter));
            const float c = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(RayOriginToSphereCenter)) - (InRadius * InRadius);
            const float discriminant = h * h - a * c;
            if (discriminant < 0) {
                return -1.0f;
            }
            else {
                return (h - R_SQRTF(discriminant)) / a;
            }
        }

		inline static bool XM_CALLCONV RaySphereCollide(DirectX::FXMVECTOR InRayOrigin, DirectX::FXMVECTOR InRayDirection, DirectX::FXMVECTOR InSphereCenter, const float InRadius)
		{
            return SphereCollisionOnRay(InRayOrigin, InRayDirection, InSphereCenter, InRadius) >= 0.f;
        }
	};
};