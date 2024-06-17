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
};