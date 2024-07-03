#include "App/Maths/Camera.h"
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
		vec3 direction = random_on_hemisphere(vec3(hitRecord.normal));
		return 0.5 * ray_colorFLTPrecision(ray(vec3(hitRecord.p), direction), InDepth - 1, world);
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

	InOutPDestination->AspectRatio = InPSource->AspectRatio;
	InOutPDestination->FocalLength = InPSource->FocalLength;
	InOutPDestination->SamplesPerPixel = InPSource->SamplesPerPixel;
	InOutPDestination->PixelSamplesScale = InPSource->PixelSamplesScale;
	InOutPDestination->MaxDepth = InPSource->MaxDepth;
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

	InOutPDestination->AspectRatio = InPSource->AspectRatio;
	InOutPDestination->FocalLength = InPSource->FocalLength;
	InOutPDestination->SamplesPerPixel = InPSource->SamplesPerPixel;
	InOutPDestination->PixelSamplesScale = InPSource->PixelSamplesScale;
	InOutPDestination->MaxDepth	= InPSource->MaxDepth;
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
		static thread_local LocalVectorDistributionUnitSphereDistribution distrib;
		
		XMVECTOR randomScatterNormalAtHitPoint = distrib.LambertianDistributionOnHemisphere(XMLoadFloat3(&hitRecord.normal));

		//Use this line to get RayTracingInOneWeekend color result
		//randomNormalAtHitPoint = DEBUG_ToRightHandedCartesianCoordinate(randomNormalAtHitPoint);
		RayVECAnyNrm rayDepth
		{
			.Origin = XMLoadFloat3(&hitRecord.p),
			.Direction = randomScatterNormalAtHitPoint
		};
		return XMVectorScale(RayColor(&rayDepth, InDepth - 1, InWorld), 0.5f );
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
	m_cameraData.ImageSizeFromWidth(m_cameraData.ImageSize.x);
	m_cameraData.ViewportSizeFromHeight(2.f);
	m_cameraData.PixelSamplesScale = 1.f / (float)m_cameraData.SamplesPerPixel;

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
			XMStoreFloat3(&InTarget->operator[]({ x, y }), XMVectorScale(resultColor, m_cameraData.PixelSamplesScale));

#endif
		}
	}
}
