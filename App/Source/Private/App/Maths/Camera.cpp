#include "App/Maths/Camera.h"
#include "App/Maths/Interval.h"
#include "App/Maths/RMath.h"
#include "App/Maths/Ray.h"
#include "App/Hittables/HList.h"
#include "App/SystemElement/Picture.h"
#include "App/Tools/RLog.h"
#include "App/Debug/DebugMath.h"

using namespace AppNmsp;
using namespace DirectX;

CameraFLT CameraFLT::FromSelectedCameraData(const float InAspectRation, const float InFocalLength, const XMFLOAT3& InCameraCenter, const int32_t InImageWidth, const float InViewportHeigth)
{
	CameraFLT result;
	result.AspectRatio = InAspectRation;
	result.FocalLength = InFocalLength;
	result.ImageSize.x = InImageWidth;
	result.ViewportSize.y = InViewportHeigth;
	result.CameraCenter = InCameraCenter;

	result.ImageSize.y = int32_t((float)result.ImageSize.x / result.AspectRatio);
	result.ViewportSize.x = result.ViewportSize.y * ((float)result.ImageSize.x / (float)result.ImageSize.y);

	// Calculate the vectors across the horizontal and down the vertical viewport edges.
	result.ViewportU = XMFLOAT3(0.f, result.ViewportSize.x, 0.f);
	result.ViewportV = XMFLOAT3(0.f, 0.f, -result.ViewportSize.y);


	XMVECTOR lCameraViewportSize = XMLoadFloat2(&result.ViewportSize);

	XMVECTOR lCameraCenter = XMLoadFloat3(&result.CameraCenter);
	XMVECTOR lCameraViewportU = XMLoadFloat3(&result.ViewportU);
	XMVECTOR lCameraViewportV = XMLoadFloat3(&result.ViewportV);

	XMVECTOR lCameraImageSize{ (float)result.ImageSize.x, (float)result.ImageSize.y, 0.f, 0.f };


	// Calculate the horizontal and vertical delta vectors from pixel to pixel.
	XMVECTOR lCameraPixelDeltaU = XMVectorScale(lCameraViewportU, 1.f/(float)result.ImageSize.x);
	XMVECTOR lCameraPixelDeltaV = XMVectorScale(lCameraViewportV, 1.f/(float)result.ImageSize.y);
	XMStoreFloat3(&result.PixelDeltaU, lCameraPixelDeltaU);
	XMStoreFloat3(&result.PixelDeltaV, lCameraPixelDeltaV);

	XMVECTOR lFocalVector{ result.FocalLength, 0.f, 0.f, 0.f };

	// Calculate the location of the upper left pixel.
	XMVECTOR lViewportUpperLeftPos = lCameraCenter + lFocalVector - (lCameraViewportU / 2.f) - (lCameraViewportV / 2.f);
	XMStoreFloat3(&result.ViewportUpperLeftPos, lViewportUpperLeftPos);

	XMVECTOR lPixel00Pos = lViewportUpperLeftPos + XMVectorScale((lCameraPixelDeltaU + lCameraPixelDeltaV), 0.5f);
	XMStoreFloat3(&result.Pixel00Pos, lPixel00Pos);

	return result;
}

void AppNmsp::XMLoadCamera(CameraVEC* InOutPDestination, const CameraFLT* InPSource)
{
	InOutPDestination->CameraCenter			= DirectX::XMLoadFloat3(&InPSource->CameraCenter);
	InOutPDestination->ViewportU			= DirectX::XMLoadFloat3(&InPSource->ViewportU);
	InOutPDestination->ViewportV			= DirectX::XMLoadFloat3(&InPSource->ViewportV);
	InOutPDestination->PixelDeltaU			= DirectX::XMLoadFloat3(&InPSource->PixelDeltaU);
	InOutPDestination->PixelDeltaV			= DirectX::XMLoadFloat3(&InPSource->PixelDeltaV);
	InOutPDestination->ViewportUpperLeftPos	= DirectX::XMLoadFloat3(&InPSource->ViewportUpperLeftPos);
	InOutPDestination->Pixel00Pos			= DirectX::XMLoadFloat3(&InPSource->Pixel00Pos);

	InOutPDestination->ViewportSize			= DirectX::XMLoadFloat2(&InPSource->ViewportSize);

	InOutPDestination->ImageSize			= DirectX::XMLoadSInt2(&InPSource->ImageSize);
}

void AppNmsp::XMStoreCamera(CameraFLT* InOutPDestination, const CameraVEC* InPSource)
{
	DirectX::XMStoreFloat3(&InOutPDestination->CameraCenter			, InPSource->CameraCenter);
	DirectX::XMStoreFloat3(&InOutPDestination->ViewportU			, InPSource->ViewportU);
	DirectX::XMStoreFloat3(&InOutPDestination->ViewportV			, InPSource->ViewportV);
	DirectX::XMStoreFloat3(&InOutPDestination->PixelDeltaU			, InPSource->PixelDeltaU);
	DirectX::XMStoreFloat3(&InOutPDestination->PixelDeltaV			, InPSource->PixelDeltaV);
	DirectX::XMStoreFloat3(&InOutPDestination->ViewportUpperLeftPos	, InPSource->ViewportUpperLeftPos);
	DirectX::XMStoreFloat3(&InOutPDestination->Pixel00Pos			, InPSource->Pixel00Pos);

	DirectX::XMStoreFloat2(&InOutPDestination->ViewportSize			, InPSource->ViewportSize);

	DirectX::XMStoreSInt2(&InOutPDestination->ImageSize				, InPSource->ImageSize);
}



inline XMFLOAT3 XM_CALLCONV RayColor(const RayVECAnyNrm* InPlRay, const HList* InWorld)
{
	static constexpr Interval DefaultInterval = Interval(0, R_INFINITY_F);
	XMFLOAT3 result;
	HitRecord hitRecord;

	if (InWorld->Hit(*InPlRay, DefaultInterval, /*Out*/ hitRecord))
	{
		XMVECTOR NormalAtHitPoint = XMLoadFloat3(&hitRecord.normal);

		//Use this line to get RayTracingInOneWeekend color result
		NormalAtHitPoint = DEBUG_ToRightHandedCartesianCoordinate(NormalAtHitPoint);

		XMStoreFloat3(&result, XMVectorScale(NormalAtHitPoint + VECTOR_ONE, 0.5f));
	}
	else
	{
		XMVECTOR rayNorm = XMVector3Normalize(InPlRay->Direction);

		float a = 0.5f * (XMVectorGetZ(rayNorm) + 1.f);
		static constexpr XMVECTOR ColA{ 1.f, 1.f, 1.f, 0.f };
		static constexpr XMVECTOR ColB{ 0.5f, 0.7f, 1.f, 1.f };

		XMStoreFloat3(&result, ((1.f - a) * ColA) + (a * ColB));
	}
	return result;
}

void AppNmsp::Camera::Initialize()
{
	m_cameraData.ImageSizeFromWidth(m_cameraData.ImageSize.x);
	m_cameraData.ViewportSizeFromHeight(2.f);


	// Calculate the vectors across the horizontal and down the vertical viewport edges.
	m_cameraData.ViewportU = XMFLOAT3(0.f, m_cameraData.ViewportSize.x, 0.f);
	m_cameraData.ViewportV = XMFLOAT3(0.f, 0.f, -m_cameraData.ViewportSize.y);



	XMVECTOR lCameraCenter = XMLoadFloat3(&m_cameraData.CameraCenter);
	XMVECTOR lCameraViewportU = XMLoadFloat3(&m_cameraData.ViewportU);
	XMVECTOR lCameraViewportV = XMLoadFloat3(&m_cameraData.ViewportV);


	// Calculate the horizontal and vertical delta vectors from pixel to pixel.
	XMVECTOR lCameraPixelDeltaU = XMVectorScale(lCameraViewportU, 1.f / (float)m_cameraData.ImageSize.x);
	XMVECTOR lCameraPixelDeltaV = XMVectorScale(lCameraViewportV, 1.f / (float)m_cameraData.ImageSize.y);

	XMStoreFloat3(&m_cameraData.PixelDeltaU, lCameraPixelDeltaU);
	XMStoreFloat3(&m_cameraData.PixelDeltaV, lCameraPixelDeltaV);

	XMVECTOR lFocalVector{ m_cameraData.FocalLength, 0.f, 0.f, 0.f };

	// Calculate the location of the upper left pixel.
	XMVECTOR lViewportUpperLeftPos = lCameraCenter + lFocalVector - (lCameraViewportU / 2.f) - (lCameraViewportV / 2.f);
	XMStoreFloat3(&m_cameraData.ViewportUpperLeftPos, lViewportUpperLeftPos);

	XMVECTOR lPixel00Pos = lViewportUpperLeftPos + XMVectorScale((lCameraPixelDeltaU + lCameraPixelDeltaV), 0.5f);
	XMStoreFloat3(&m_cameraData.Pixel00Pos, lPixel00Pos);

	m_bInitilized = true;
}

void AppNmsp::Camera::Render(const HList* InWorld, Picture* InTarget)
{
#if _DEBUG
	if (!m_bInitilized)
	{
		RLog::Log(LOG_EXCEPTION, TEXT("Cannot render with an unitialized camera !"));
		return;
	}
#endif // _DEBUG

	XMINT2 size = InTarget->GetSize();
	XMFLOAT2 sizef = XMFLOAT2((float)size.x, (float)size.y);


	XMVECTOR lPixel00Pos = XMLoadFloat3(&m_cameraData.Pixel00Pos);
	XMVECTOR lPixelDeltaU = XMLoadFloat3(&m_cameraData.PixelDeltaU);
	XMVECTOR lPixelDeltaV = XMLoadFloat3(&m_cameraData.PixelDeltaV);
	XMVECTOR lCameraCenter = XMLoadFloat3(&m_cameraData.CameraCenter);
	for (int32_t y = 0; y < size.y; ++y)
	{
		static int32_t lastLogs = y / 10;
		if ((y / 10) > lastLogs)
		{
			RLog::Log(LOG_DISPLAY, TEXT("In Row y = '{}'"), y);
			lastLogs = y / 10;
		}
		float yf = (float)y;
		for (int32_t x = 0; x < size.x; ++x)
		{
			float xf = (float)x;
			XMVECTOR lPixelCenter = lPixel00Pos + (xf * lPixelDeltaU) + (yf * lPixelDeltaV);
			XMVECTOR lRayDir = lPixelCenter - lCameraCenter;
			RayVECLength lRay
			{
				.Origin = lCameraCenter,
				.Direction = lRayDir
			};

			InTarget->operator[]({x, y}) = RayColor(&lRay, InWorld);
		}
	}
}
