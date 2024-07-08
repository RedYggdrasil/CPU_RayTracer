#include "App/Maths/Camera.h"

#include "App/Assets/Materials/Material.h"
#include "App/Maths/Interval.h"
#include "App/Maths/RMath.h"
#include "App/Maths/RMathRand.h"
#include "App/Maths/Ray.h"
#include "App/Hittables/HList.h"
#include "App/SystemElement/Picture.h"
#include "App/Tools/RLog.h"
#include "App/Tools/RRenderRandomizer.h"
#include "App/Debug/DebugMath.h"
#include <iostream>

using namespace AppNmsp;
using namespace DirectX;



#if WITH_REFERENCE
ray get_ray(const Camera& InCamera, int i, int j) {
	// Construct a camera ray originating from the origin and directed at randomly sampled
	// point around the pixel location i, j.

	vec3 offset = sample_square();
	vec3 pixel_sample = vec3(InCamera.GetData().Pixel00Pos)
		+ ((i + offset.x()) * InCamera.GetData().PixelDeltaU)
		+ ((j + offset.y()) * InCamera.GetData().PixelDeltaV);

	vec3 ray_origin = vec3(InCamera.GetData().CameraCenter);
	vec3 ray_direction = pixel_sample - ray_origin;

	return ray(ray_origin, ray_direction);
}


color ray_color(const ray& r, const int32_t InDepth, const HList* world) 
{
	if (InDepth <= 0)
	{
		return color(0.0, 0.0, 0.0);
	}
	hit_record hitRecord;
	if (world->Hit(r, DInterval(0, std::numeric_limits<double>::infinity()), hitRecord)) {
		ray scattered;
		color attenuation;
		if (hitRecord.mat->scatter(r, hitRecord, attenuation, scattered))
			return attenuation * ray_color(scattered, InDepth - 1, world);
		return color(0, 0, 0);

		vec3 direction = random_on_hemisphere(hitRecord.normal);
		return 0.5 * ray_color(ray(hitRecord.p, direction), InDepth - 1, world);
	}

	vec3 unit_direction = unit_vector(r.direction());
	auto a = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}
color ray_colorFLTPrecision(const ray& r, const int32_t InDepth, const HList* world)
{
	if (InDepth <= 0)
	{
		return color(0.0, 0.0, 0.0);
	}
	HitRecord hitRecord;
	RayVEC rv;
	r.ToRayVEC(&rv);
	if (world->Hit(rv, FInterval(0, R_INFINITY_F), hitRecord)) {
		vec3 direction = random_on_hemisphere(vec3(hitRecord.SurfaceNormal));
		return 0.5 * ray_colorFLTPrecision(ray(vec3(hitRecord.ImpactPoint), direction), InDepth - 1, world);
	}

	vec3 unit_direction = unit_vector(r.direction());
	auto a = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}
#endif WITH_REFERENCE

CameraFLT CameraFLT::FromSelectedCameraData(const float InAspectRation, const float InFocalLength, const XMFLOAT3& InCameraCenter, const int32_t InImageWidth, const float InViewportHeigth)
{
	CameraFLT result;
	result.AspectRatio = InAspectRation;
	result.m_focalLength = InFocalLength;
	result.m_imageSize.x = InImageWidth;
	result.m_viewportSize.y = InViewportHeigth;
	result.m_cameraCenter = InCameraCenter;

	result.m_imageSize.y = int32_t((float)result.m_imageSize.x / result.AspectRatio);
	result.m_viewportSize.x = result.m_viewportSize.y * ((float)result.m_imageSize.x / (float)result.m_imageSize.y);

	// Calculate the vectors across the horizontal and down the vertical viewport edges.
	result.m_viewportU = XMFLOAT3(0.f, result.m_viewportSize.x, 0.f);
	result.m_viewportV = XMFLOAT3(0.f, 0.f, -result.m_viewportSize.y);


	XMVECTOR lCameraViewportSize = XMLoadFloat2(&result.m_viewportSize);

	XMVECTOR lCameraCenter = XMLoadFloat3(&result.m_cameraCenter);
	XMVECTOR lCameraViewportU = XMLoadFloat3(&result.m_viewportU);
	XMVECTOR lCameraViewportV = XMLoadFloat3(&result.m_viewportV);

	XMVECTOR lCameraImageSize{ (float)result.m_imageSize.x, (float)result.m_imageSize.y, 0.f, 0.f };


	// Calculate the horizontal and vertical delta vectors from pixel to pixel.
	XMVECTOR lCameraPixelDeltaU = XMVectorScale(lCameraViewportU, 1.f/(float)result.m_imageSize.x);
	XMVECTOR lCameraPixelDeltaV = XMVectorScale(lCameraViewportV, 1.f/(float)result.m_imageSize.y);
	XMStoreFloat3(&result.m_pixelDeltaU, lCameraPixelDeltaU);
	XMStoreFloat3(&result.m_pixelDeltaV, lCameraPixelDeltaV);

	XMVECTOR lFocalVector{ result.m_focalLength, 0.f, 0.f, 0.f };

	// Calculate the location of the upper left pixel.
	XMVECTOR lViewportUpperLeftPos = lCameraCenter + lFocalVector - (lCameraViewportU / 2.f) - (lCameraViewportV / 2.f);
	XMStoreFloat3(&result.m_viewportUpperLeftPos, lViewportUpperLeftPos);

	XMVECTOR lPixel00Pos = lViewportUpperLeftPos + XMVectorScale((lCameraPixelDeltaU + lCameraPixelDeltaV), 0.5f);
	XMStoreFloat3(&result.m_pixel00Pos, lPixel00Pos);

	return result;
}

void AppNmsp::XMLoadCamera(CameraVEC* InOutPDestination, const CameraFLT* InPSource)
{
	//12 Bytes privates
	InOutPDestination->CameraCenter			= DirectX::XMLoadFloat3(&InPSource->m_cameraCenter);
	InOutPDestination->ViewportU			= DirectX::XMLoadFloat3(&InPSource->m_viewportU);
	InOutPDestination->ViewportV			= DirectX::XMLoadFloat3(&InPSource->m_viewportV);
	InOutPDestination->PixelDeltaU			= DirectX::XMLoadFloat3(&InPSource->m_pixelDeltaU);
	InOutPDestination->PixelDeltaV			= DirectX::XMLoadFloat3(&InPSource->m_pixelDeltaV);
	InOutPDestination->ViewportUpperLeftPos	= DirectX::XMLoadFloat3(&InPSource->m_viewportUpperLeftPos);
	InOutPDestination->Pixel00Pos			= DirectX::XMLoadFloat3(&InPSource->m_pixel00Pos);
	InOutPDestination->CameraU				= DirectX::XMLoadFloat3(&InPSource->m_cameraU);
	InOutPDestination->CameraV				= DirectX::XMLoadFloat3(&InPSource->m_cameraV);
	InOutPDestination->CameraW				= DirectX::XMLoadFloat3(&InPSource->m_cameraW);

	//12 Bytes publics
	InOutPDestination->PosLookFrom			= DirectX::XMLoadFloat3(&InPSource->PosLookFrom);
	InOutPDestination->PosLookAt			= DirectX::XMLoadFloat3(&InPSource->PosLookAt);
	InOutPDestination->UpVec				= DirectX::XMLoadFloat3(&InPSource->UpVec);

	//8 Bytes privates
	InOutPDestination->ViewportSize			= DirectX::XMLoadFloat2(&InPSource->m_viewportSize);
	InOutPDestination->ImageSize			= DirectX::XMLoadSInt2(&InPSource->m_imageSize);

	//4 Bytes privates
	InOutPDestination->PixelSamplesScale	= InPSource->m_pixelSamplesScale;
	InOutPDestination->FocalLength			= InPSource->m_focalLength;

	//4 Bytes publics
	InOutPDestination->AspectRatio			= InPSource->AspectRatio;
	InOutPDestination->SamplesPerPixel		= InPSource->SamplesPerPixel;
	InOutPDestination->MaxDepth				= InPSource->MaxDepth;
	InOutPDestination->vFov					= InPSource->vFov;
}

void AppNmsp::XMStoreCamera(CameraFLT* InOutPDestination, const CameraVEC* InPSource)
{
	//12 Bytes privates
	DirectX::XMStoreFloat3(&InOutPDestination->m_cameraCenter			, InPSource->CameraCenter);
	DirectX::XMStoreFloat3(&InOutPDestination->m_viewportU				, InPSource->ViewportU);
	DirectX::XMStoreFloat3(&InOutPDestination->m_viewportV				, InPSource->ViewportV);
	DirectX::XMStoreFloat3(&InOutPDestination->m_pixelDeltaU			, InPSource->PixelDeltaU);
	DirectX::XMStoreFloat3(&InOutPDestination->m_pixelDeltaV			, InPSource->PixelDeltaV);
	DirectX::XMStoreFloat3(&InOutPDestination->m_viewportUpperLeftPos	, InPSource->ViewportUpperLeftPos);
	DirectX::XMStoreFloat3(&InOutPDestination->m_pixel00Pos				, InPSource->Pixel00Pos);
	DirectX::XMStoreFloat3(&InOutPDestination->m_cameraU				, InPSource->CameraU);
	DirectX::XMStoreFloat3(&InOutPDestination->m_cameraV				, InPSource->CameraV);
	DirectX::XMStoreFloat3(&InOutPDestination->m_cameraW				, InPSource->CameraW);

	//12 Bytes publics
	DirectX::XMStoreFloat3(&InOutPDestination->PosLookFrom				, InPSource->PosLookFrom);
	DirectX::XMStoreFloat3(&InOutPDestination->PosLookAt				, InPSource->PosLookAt);
	DirectX::XMStoreFloat3(&InOutPDestination->UpVec					, InPSource->UpVec);

	//8 Bytes privates
	DirectX::XMStoreFloat2(&InOutPDestination->m_viewportSize			, InPSource->ViewportSize);
	DirectX::XMStoreSInt2(&InOutPDestination->m_imageSize				, InPSource->ImageSize);

	//4 Bytes privates
	InOutPDestination->m_pixelSamplesScale								= InPSource->PixelSamplesScale;
	InOutPDestination->m_focalLength									= InPSource->FocalLength;

	//4 Bytes publics
	InOutPDestination->AspectRatio										= InPSource->AspectRatio;
	InOutPDestination->SamplesPerPixel									= InPSource->SamplesPerPixel;
	InOutPDestination->MaxDepth											= InPSource->MaxDepth;
	InOutPDestination->vFov												= InPSource->vFov;
}

inline XMVECTOR XM_CALLCONV SampleSquare(FXMVECTOR InPixelDeltaU, FXMVECTOR InPixelDeltaV)
{
	static thread_local RRenderRandomizer::LocalRealDistribution<float> localDistrib = RRenderRandomizer::s_GetLocalDesyncDistribution<float>(-0.5f, 0.5f);
	return (XMVectorScale(InPixelDeltaU, localDistrib()) + (XMVectorScale(InPixelDeltaV, localDistrib())));
}

XMVECTOR XM_CALLCONV RayColor(const RayVECAnyNrm* InPlRay, const int32_t InDepth, const HList* InWorld)
{
	if (InDepth <= 0) { return VECTOR_ZERO; }
	static constexpr FInterval DefaultInterval = FInterval(OBJECT_RAY_INTERVAL_AMOUNT, R_INFINITY_F);
	HitRecord hitRecord;

	if (InWorld->Hit(*InPlRay, DefaultInterval, /*Out*/ hitRecord))
	{
		RayVECAnyNrm rayScaterred;
		DirectX::XMFLOAT3 colorAttenuation;

		if (hitRecord.SurfaceMaterial->Scatter(*InPlRay, hitRecord, /*Out*/colorAttenuation, /*Out*/rayScaterred))
		{
			XMVECTOR lColorAttenuation = XMLoadFloat3(&colorAttenuation);
			return lColorAttenuation * RayColor(&rayScaterred, InDepth - 1, InWorld);
		};
		return VECTOR_ZERO;
	}
	else
	{
		XMVECTOR rayNorm = XMVector3Normalize(InPlRay->Direction);

		float a = 0.5f * (XMVectorGetZ(rayNorm) + 1.f);
		static constexpr XMVECTOR ColA{ 1.f, 1.f, 1.f, 0.f };
		static constexpr XMVECTOR ColB{ 0.5f, 0.7f, 1.f, 1.f };

		return ((1.f - a) * ColA) + (a * ColB);
	}
}

void AppNmsp::Camera::Initialize()
{
	m_cameraData.ImageSizeFromWidth(m_cameraData.m_imageSize.x);
	m_cameraData.m_pixelSamplesScale = 1.f / (float)m_cameraData.SamplesPerPixel;

	m_cameraData.m_cameraCenter = m_cameraData.PosLookFrom;


	XMVECTOR lCameraLookFrom = XMLoadFloat3(&m_cameraData.PosLookFrom);
	XMVECTOR lCameraLookAt = XMLoadFloat3(&m_cameraData.PosLookAt);
	XMVECTOR lCameraUpVec = XMLoadFloat3(&m_cameraData.UpVec);

	XMVECTOR lFocalVector = (lCameraLookAt - lCameraLookFrom);
	m_cameraData.m_focalLength = XMVectorGetX(XMVector3Length(lFocalVector));

	float vpVerticalTheta = Deg2Rad(m_cameraData.vFov);
	float halfHeight = tanf(vpVerticalTheta / 2.f);
	float viewport_height = 2.f * halfHeight * m_cameraData.m_focalLength;
	m_cameraData.ViewportSizeFromHeight(viewport_height);

	//Forward Vector					//using focal Vec
	XMVECTOR lCameraVecW = XMVector3Normalize(lFocalVector);
	//Right Vector						//as cross of given Up direction and foward vector
	XMVECTOR lCameraVecU = XMVector3Cross(lCameraUpVec, lCameraVecW);
	//Up Vector							//from computed forward and right vector
	XMVECTOR lCameraVecV = XMVector3Cross(lCameraVecW, lCameraVecU);

	XMStoreFloat3(&m_cameraData.m_cameraU, lCameraVecU);
	XMStoreFloat3(&m_cameraData.m_cameraV, lCameraVecV);
	XMStoreFloat3(&m_cameraData.m_cameraW, lCameraVecW);


	XMVECTOR lCameraViewportU =  lCameraVecU * m_cameraData.m_viewportSize.x;
	XMVECTOR lCameraViewportV = -lCameraVecV * m_cameraData.m_viewportSize.y;
	XMStoreFloat3(&m_cameraData.m_viewportU, lCameraViewportU);
	XMStoreFloat3(&m_cameraData.m_viewportV, lCameraViewportV);


	XMVECTOR lCameraCenter = XMLoadFloat3(&m_cameraData.m_cameraCenter);


	// Calculate the horizontal and vertical delta vectors from pixel to pixel.
	XMVECTOR lCameraPixelDeltaU = XMVectorScale(lCameraViewportU, 1.f / (float)m_cameraData.m_imageSize.x);
	XMVECTOR lCameraPixelDeltaV = XMVectorScale(lCameraViewportV, 1.f / (float)m_cameraData.m_imageSize.y);

	XMStoreFloat3(&m_cameraData.m_pixelDeltaU, lCameraPixelDeltaU);
	XMStoreFloat3(&m_cameraData.m_pixelDeltaV, lCameraPixelDeltaV);


	// Calculate the location of the upper left pixel.
	XMVECTOR lViewportUpperLeftPos = lCameraCenter + lFocalVector - (lCameraViewportU / 2.f) - (lCameraViewportV / 2.f);
	XMStoreFloat3(&m_cameraData.m_viewportUpperLeftPos, lViewportUpperLeftPos);

	XMVECTOR lPixel00Pos = lViewportUpperLeftPos + XMVectorScale((lCameraPixelDeltaU + lCameraPixelDeltaV), 0.5f);
	XMStoreFloat3(&m_cameraData.m_pixel00Pos, lPixel00Pos);

	m_bInitilized = true;
}

void AppNmsp::Camera::Render(const HList* InWorld, Picture* InTarget) const
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


	XMVECTOR lPixel00Pos = XMLoadFloat3(&m_cameraData.m_pixel00Pos);
	XMVECTOR lPixelDeltaU = XMLoadFloat3(&m_cameraData.m_pixelDeltaU);
	XMVECTOR lPixelDeltaV = XMLoadFloat3(&m_cameraData.m_pixelDeltaV);
	XMVECTOR lCameraCenter = XMLoadFloat3(&m_cameraData.m_cameraCenter);
	for (int32_t y = 0; y < size.y; ++y)
	{
		static int32_t lastLogs = y / 10;
		if (false && (y / 10) > lastLogs)
		{
			RLog::Log(LOG_DISPLAY, TEXT("In Row y = '{}'"), y);
			lastLogs = y / 10;
		}
		float yf = (float)y;
		for (int32_t x = 0; x < size.x; ++x)
		{
			float xf = (float)x;
			XMVECTOR lPixelCenter = lPixel00Pos + (xf * lPixelDeltaU) + (yf * lPixelDeltaV);

#if USE_DOUBLE_PRECISION
			color pixel_color(0, 0, 0);
#endif
			XMVECTOR resultColor = { 0.f, 0.f, 0.f, 0.f };
			for (int32_t sample = 0; sample < m_cameraData.SamplesPerPixel; sample++) 
			{
				XMVECTOR lRayDir = /*PixelSample*/(lPixelCenter + SampleSquare(lPixelDeltaU, lPixelDeltaV)) - /*RayOrigin*/lCameraCenter;
				RayVECLength lRay
				{
					.Origin = lCameraCenter,
					.Direction = lRayDir
				};
#if USE_DOUBLE_PRECISION
				ray rVec3 = get_ray(*this, x, y);
				pixel_color += ray_colorFLTPrecision(rVec3, m_cameraData.MaxDepth, InWorld);
#else
				resultColor += RayColor(&lRay, m_cameraData.MaxDepth, InWorld);
#endif
			}
#if USE_DOUBLE_PRECISION
			XMFLOAT3 resFLT3 = pixel_color.ToFLT3();
			pixel_color = pixel_color * m_cameraData.PixelSamplesScale;
			resultColor = XMLoadFloat3(&resFLT3);
			InTarget->m_pixelDBLs[InTarget->mPixelDBLIDX(x, y)] = pictureColor{ pixel_color.e[0], pixel_color.e[1], pixel_color.e[2] };
#else			
			XMStoreFloat3(&InTarget->operator[]({ x, y }), XMVectorScale(resultColor, m_cameraData.m_pixelSamplesScale));

#endif
		}
	}
}
