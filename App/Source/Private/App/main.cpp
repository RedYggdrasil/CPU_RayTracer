#include "App/pch.h"
#include "Gen_App/Config/AppConfig.h"

#include "App/Maths/Camera.h"
#include "App/Maths/Ray.h"
#include "App/SystemElement/Picture.h"
#include "App/Tools/RLog.h"

#include <iostream>


#include "Tracy.hpp"
#include "TracyC.h"

using namespace AppNmsp;
using namespace DirectX;

inline XMFLOAT3 XM_CALLCONV RayColor(const RayVEC* InPlRay, const int32_t InRow) {
	XMFLOAT3 result = XMFLOAT3(0.f, 0.f, 0.f);
	XMVECTOR rayNorm = XMVector3Normalize(InPlRay->Direction);
	
	float a = 0.5f * (XMVectorGetZ(rayNorm) + 1.f);

	static const XMVECTOR ColA{ 1.f, 1.f, 1.f, 0.f };
	static const XMVECTOR ColB{ 0.5f, 0.7f, 1.f, 1.f };
	XMStoreFloat3(&result, ((1.f - a) * ColA) + (a * ColB));
	return result;
}

int main(int argc, char** argv) 
{
	ZoneScopedN("Application");


	Camera cameraData = Camera::FromSelectedCameraData(16.f / 9.f, 1.f, XMFLOAT3(0.f, 0.f, 0.f), 400, 2.f);
	CameraVEC lCameraData;
	XMLoadCamera(&lCameraData, &cameraData);


	Picture resultBuffer(cameraData.ImageSize, TEXT("Result.ppm"));



	{
		ZoneScopedN("Draw");
		XMINT2 size = resultBuffer.GetSize();
		XMFLOAT2 sizef = XMFLOAT2((float)size.x, (float)size.y);

		RLog::Log(LOG_DISPLAY, TEXT("Pixel00Pos = '{}'"), _tostring(cameraData.Pixel00Pos));
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
				XMVECTOR lPixelCenter = lCameraData.Pixel00Pos + (xf * lCameraData.PixelDeltaU) + (yf * lCameraData.PixelDeltaV);
				XMVECTOR lRayDir = lPixelCenter - lCameraData.CameraCenter;
				RayVEC lRay
				{
					.Origin = lCameraData.CameraCenter,
					.Direction = lRayDir
				};

				resultBuffer[{x, y}] = RayColor(&lRay, yf);
			}
		}
	}

	{
		ZoneScopedN("Write to disk");
		resultBuffer.WriteToDisk(true);
	}
	char r;
	std::cin >> r;

	return 0;
}