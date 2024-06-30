#pragma once
#include "App/pch.h"

namespace AppNmsp
{
	class __declspec(align(16)) RayVEC
	{
	public:
		DirectX::XMVECTOR Origin;
		DirectX::XMVECTOR Direction;
	public:
		inline DirectX::XMVECTOR XM_CALLCONV At(const float InProgress) const
		{
			return DirectX::XMVectorAdd(Origin, DirectX::XMVectorScale(Direction, InProgress));
		};
	};

	//A RayVec with normalized Direction
	using RayVECNrmlzd = RayVEC;
	//A RayVec with length Direction
	using RayVECLength = RayVEC;
	//A RayVec with normalized or length Direction
	using RayVECAnyNrm = RayVEC;

	class RayFLT
	{
	public:
		DirectX::XMFLOAT3 Origin;
		DirectX::XMFLOAT3 Direction;
	public:
		inline void LoadToVector(RayVEC* InOutPDestination) const
		{
			InOutPDestination->Origin = DirectX::XMLoadFloat3(&Origin);
			InOutPDestination->Direction = DirectX::XMLoadFloat3(&Direction);
		};
		inline void StoreFromVector(const RayVEC* InPSource)
		{
			DirectX::XMStoreFloat3(&Origin, InPSource->Origin);
			DirectX::XMStoreFloat3(&Direction, InPSource->Direction);
		};
	};
	//A RayFLT with normalized Direction
	using RayFLTNrmlzd = RayFLT;
	//A RayFLT with length Direction
	using RayFLTLength = RayFLT;
	//A RayFLT with normalized or length Direction
	using RayFLTAnyNrm = RayFLT;

	inline void XMLoadRay(RayVEC* InOutPDestination, const RayFLT* InOutPSource)
	{
		InOutPDestination->Origin =		DirectX::XMLoadFloat3(&InOutPSource->Origin);
		InOutPDestination->Direction =	DirectX::XMLoadFloat3(&InOutPSource->Direction);
	};
	inline void XMStoreRay(RayFLT*  InOutPDestination, const RayVEC* InOutPSource)
	{
		DirectX::XMStoreFloat3(&InOutPDestination->Origin,		InOutPSource->Origin);
		DirectX::XMStoreFloat3(&InOutPDestination->Direction,	InOutPSource->Direction);
	};

	std::string _tostring(const RayFLT& InRayFLT);
};

#if WITH_REFERENCE
class ray {
public:
	ray() {}

	ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

	const point3& origin() const { return orig; }
	const vec3& direction() const { return dir; }

	point3 at(double t) const {
		return orig + t * dir;
	}
	AppNmsp::RayFLT ToRayFLT() const
	{
		return AppNmsp::RayFLT
		{
			.Origin = orig.ToFLT3(),
			.Direction = dir.ToFLT3(),
		};
	}
	void ToRayVEC(AppNmsp::RayVEC* InOutRayVec) const
	{
		AppNmsp::RayFLT src
		{
			.Origin = orig.ToFLT3(),
			.Direction = dir.ToFLT3(),
		};
		XMLoadRay(InOutRayVec, &src);
	}
private:
	point3 orig;
	vec3 dir;
};
#endif