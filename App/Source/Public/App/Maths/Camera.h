#pragma once
#include "App/pch.h"

namespace AppNmsp
{
	struct __declspec(align(16)) CameraVEC
	{
	public:
		DirectX::XMVECTOR CameraCenter;
		DirectX::XMVECTOR ViewportU;
		DirectX::XMVECTOR ViewportV;
		DirectX::XMVECTOR PixelDeltaU;
		DirectX::XMVECTOR PixelDeltaV;
		DirectX::XMVECTOR ViewportUpperLeftPos;
		DirectX::XMVECTOR Pixel00Pos;
		DirectX::XMVECTOR CameraU; //Camera Right Vector (y)
		DirectX::XMVECTOR CameraV; //Camera Up Vector (z)
		DirectX::XMVECTOR CameraW; //Camera Forward Vector (x)

		DirectX::XMVECTOR PosLookFrom;
		DirectX::XMVECTOR PosLookAt;
		DirectX::XMVECTOR UpVec;

		DirectX::XMVECTOR ViewportSize;
		DirectX::XMVECTOR ImageSize;

		float PixelSamplesScale;
		float FocalLength;

		float AspectRatio;
		int32_t SamplesPerPixel;
		int32_t MaxDepth;
		float vFov;
	};
	struct CameraFLT
	{
		//12 Bytes vars
	private:
		DirectX::XMFLOAT3	m_cameraCenter;
		DirectX::XMFLOAT3	m_viewportU;
		DirectX::XMFLOAT3	m_viewportV;
		DirectX::XMFLOAT3	m_pixelDeltaU;
		DirectX::XMFLOAT3	m_pixelDeltaV;
		DirectX::XMFLOAT3	m_viewportUpperLeftPos;
		DirectX::XMFLOAT3	m_pixel00Pos;
		DirectX::XMFLOAT3	m_cameraU; //Camera Right Vector (y)
		DirectX::XMFLOAT3	m_cameraV; //Camera Up Vector (z)
		DirectX::XMFLOAT3	m_cameraW; //Camera Forward Vector (x)
	public:
		DirectX::XMFLOAT3	PosLookFrom		= DirectX::XMFLOAT3	{ 0.f, 0.f, 0.f };
		DirectX::XMFLOAT3	PosLookAt		= DirectX::XMFLOAT3	{ 1.f, 0.f, 0.f };
		DirectX::XMFLOAT3	UpVec			= DirectX::XMFLOAT3	{ 0.f, 0.f, 1.f };

		//8 Bytes vars
	private:
		DirectX::XMFLOAT2	m_viewportSize	= DirectX::XMFLOAT2	{ 2.f, 2.f };
		DirectX::XMINT2		m_imageSize		= DirectX::XMINT2	{ 100, 100 };

		//4 Bytes vars
	private:
		float				m_pixelSamplesScale;
		float				m_focalLength = 1.f;
	public:
		float				AspectRatio = 1.f;
		int32_t				SamplesPerPixel = 10;
		int32_t				MaxDepth = 10;
		float				vFov = 90.f;
	public:
		inline static DirectX::XMINT2 ImageSizeFromWidthAndAspectRatio(const int32_t InWidth, const float InAspectRatio) { return DirectX::XMINT2{ InWidth, int32_t((float)InWidth / InAspectRatio) }; }
		inline void ImageSizeFromWidth(const int32_t InWidth) { this->m_imageSize = ImageSizeFromWidthAndAspectRatio(InWidth, this->AspectRatio); }
		
		inline static DirectX::XMFLOAT2 ViewportSizeFromHeightAndImageSize(const float InHeight, const DirectX::XMINT2 InImageSize) { return DirectX::XMFLOAT2{ InHeight * ((float)InImageSize.x / (float)InImageSize.y), InHeight }; }
		inline void ViewportSizeFromHeight(const float InHeight) { this->m_viewportSize = ViewportSizeFromHeightAndImageSize(InHeight, this->m_imageSize); }
	public:
		static CameraFLT FromSelectedCameraData
		(
			const float InAspectRation, 
			const float InFocalLength, 
			const DirectX::XMFLOAT3& InCameraCenter,
			const int32_t InImageWidth,
			const float InViewportHeigth
		);
	public:
		DirectX::XMINT2 GetImageSize() const { return m_imageSize; }

	public:
		friend void XMLoadCamera(CameraVEC* InOutPDestination, const CameraFLT* InPSource);
		friend void XMStoreCamera(CameraFLT* InOutPDestination, const CameraVEC* InPSource);
		friend class Camera;
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
		void Render(const class HList* InWorld, class Picture* InTarget) const;
	};
};