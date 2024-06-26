#include "App/Maths/Camera.h"
#include "App/Maths/RMath.h"
#include "App/Tools/RLog.h"

using namespace AppNmsp;
using namespace DirectX;

Camera Camera::FromSelectedCameraData(const float InAspectRation, const float InFocalLength, const XMFLOAT3& InCameraCenter, const int32_t InImageWidth, const float InViewportHeigth)
{
	Camera result;
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

void AppNmsp::XMLoadCamera(CameraVEC* InOutPDestination, const Camera* InPSource)
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

void AppNmsp::XMStoreCamera(Camera* InOutPDestination, const CameraVEC* InPSource)
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
