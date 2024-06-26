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
		inline static bool XM_CALLCONV RaySphereCollide(const RayVECAnyNrm* InRayVec, DirectX::FXMVECTOR InSphereCenter, const float InRadius)
		{
            DirectX::XMVECTOR RayOriginToSphereCenter = DirectX::XMVectorSubtract(InSphereCenter, InRayVec->Origin);
            const float a = DirectX::XMVectorGetX(DirectX::XMVector3Dot(InRayVec->Direction, InRayVec->Direction));
            const float b = -2.f * DirectX::XMVectorGetX(DirectX::XMVector3Dot(InRayVec->Direction, RayOriginToSphereCenter));
            const float c = DirectX::XMVectorGetX(DirectX::XMVector3Dot(RayOriginToSphereCenter, RayOriginToSphereCenter)) - (InRadius * InRadius);
            const float discriminant = b * b - 4.f * a * c;
            return (discriminant >= 0.f);
        }
	};
};