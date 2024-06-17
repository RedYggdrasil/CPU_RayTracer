#include "App/pch.h"
#include "Gen_App/Config/AppConfig.h"
#include "App/Tools/RLog.h"

#include "App/SystemElement/Picture.h"

#include <iostream>


#include "Tracy.hpp"
#include "TracyC.h"

using namespace AppNmsp;
using namespace DirectX;
int main(int argc, char** argv) 
{
	ZoneScopedN("Application");

	Picture resultBuffer(XMINT2(256, 256), TEXT("Result.ppm"));

	{
		ZoneScopedN("Draw");
		XMINT2 size = resultBuffer.GetSize();
		XMFLOAT2 sizef = XMFLOAT2((float)size.x, (float)size.y);

		for (int32_t x = 0; x < size.x; ++x)
		{
			static int32_t lastLogs = x / 10;
			if ((x / 10) > lastLogs)
			{
				RLog::Log(LOG_DISPLAY, TEXT("In Row x = '{}'"), x);
				lastLogs = x / 10;
			}
			float xf = (float)x;
			for (int32_t y = 0; y < size.y; ++y)
			{
				resultBuffer[{x, y}].Color = Color
				{
					.R = (uint8_t)((xf / sizef.x) * 255.f),
					.G = (uint8_t)(((float)y / sizef.y) * 255.f),
					.B = 127ui8
				};
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