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
	struct CameraFLT
	{
	public:
		DirectX::XMFLOAT3 CameraCenter = DirectX::XMFLOAT3{ 0.f, 0.f, 0.f };
		DirectX::XMFLOAT3 ViewportU;
		DirectX::XMFLOAT3 ViewportV;
		DirectX::XMFLOAT3 PixelDeltaU;
		DirectX::XMFLOAT3 PixelDeltaV;
		DirectX::XMFLOAT3 ViewportUpperLeftPos;
		DirectX::XMFLOAT3 Pixel00Pos;
		DirectX::XMFLOAT2 ViewportSize = DirectX::XMFLOAT2{ 2.f, 2.f };
		DirectX::XMINT2 ImageSize = DirectX::XMINT2{ 100, 100 };
		float AspectRatio = 1.f;
		float FocalLength = 1.f;
	public:
		inline static DirectX::XMINT2 ImageSizeFromWidthAndAspectRatio(const int32_t InWidth, const float InAspectRatio) { return DirectX::XMINT2{ InWidth, int32_t((float)InWidth / InAspectRatio) }; }
		inline void ImageSizeFromWidth(const int32_t InWidth) { this->ImageSize = ImageSizeFromWidthAndAspectRatio(InWidth, this->AspectRatio); }
		
		inline static DirectX::XMFLOAT2 ViewportSizeFromHeightAndImageSize(const float InHeight, const DirectX::XMINT2 InImageSize) { return DirectX::XMFLOAT2{ InHeight * ((float)InImageSize.x / (float)InImageSize.y), InHeight }; }
		inline void ViewportSizeFromHeight(const float InHeight) { this->ViewportSize = ViewportSizeFromHeightAndImageSize(InHeight, this->ImageSize); }
	public:
		static CameraFLT FromSelectedCameraData
		(
			const float InAspectRation, 
			const float InFocalLength, 
			const DirectX::XMFLOAT3& InCameraCenter,
			const int32_t InImageWidth,
			const float InViewportHeigth
		);
	};

	void XMLoadCamera(CameraVEC* InOutPDestination, const CameraFLT* InPSource);
	void XMStoreCamera(CameraFLT* InOutPDestination, const CameraVEC* InPSource);

	class Camera
	{
	private :
		CameraFLT m_cameraData;
		bool m_bInitilized = false;
	public:
		inline CameraFLT& GetData() { return m_cameraData; }
		inline const CameraFLT& GetData() const { return m_cameraData; }
		inline void XMLoadCameraData(CameraVEC* InOutPDestination) const { XMLoadCamera(InOutPDestination, &m_cameraData); }
		inline void XMStoreCameraData(const CameraVEC* InPSource) { XMStoreCamera(&m_cameraData, InPSource); }
	public:
		void Initialize();
	public:
		void Render(const class HList* InWorld, class Picture* InTarget);
	};
};