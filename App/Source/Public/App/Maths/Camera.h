#pragma once
#include "App/pch.h"

namespace AppNmsp
{
	struct __declspec(align(16)) CameraVEC
	{
	public:
		DirectX::XMVECTOR CameraCenter;
		DirectX::XMVECTOR ViewportSize;
		DirectX::XMVECTOR ViewportU;
		DirectX::XMVECTOR ViewportV;
		DirectX::XMVECTOR PixelDeltaU;
		DirectX::XMVECTOR PixelDeltaV;
		DirectX::XMVECTOR ViewportUpperLeftPos;
		DirectX::XMVECTOR Pixel00Pos;
		DirectX::XMVECTOR ImageSize;
		float AspectRatio;
		float FocalLength;
	};
	struct Camera
	{
	public:
		DirectX::XMFLOAT3 CameraCenter;
		DirectX::XMFLOAT3 ViewportU;
		DirectX::XMFLOAT3 ViewportV;
		DirectX::XMFLOAT3 PixelDeltaU;
		DirectX::XMFLOAT3 PixelDeltaV;
		DirectX::XMFLOAT3 ViewportUpperLeftPos;
		DirectX::XMFLOAT3 Pixel00Pos;
		DirectX::XMFLOAT2 ViewportSize;
		DirectX::XMINT2 ImageSize;
		float AspectRatio;
		float FocalLength;
	public:
		static Camera FromSelectedCameraData
		(
			const float InAspectRation, 
			const float InFocalLength, 
			const DirectX::XMFLOAT3& InCameraCenter,
			const int32_t InImageWidth,
			const float InViewportHeigth
		);
	};

	void XMLoadCamera(CameraVEC* InOutPDestination, const Camera* InPSource);
	void XMStoreCamera(Camera* InOutPDestination, const CameraVEC* InPSource);
};