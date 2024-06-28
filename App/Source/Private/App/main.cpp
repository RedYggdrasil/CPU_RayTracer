#include "App/pch.h"
#include "Gen_App/Config/AppConfig.h"

#include "App/Maths/Camera.h"
#include "App/Maths/RMath.h"
#include "App/Maths/RMathCol.h"
#include "App/Maths/Ray.h"
#include "App/SystemElement/Picture.h"
#include "App/Tools/RRenderRandomizer.h"
#include "App/Tools/RLog.h"

#include "App/Debug/DebugMath.h"
#include "App/Hittables/HList.h"
#include "App/Hittables/HSphere.h"

#include <iostream>

#include "Tracy.hpp"
#include "TracyC.h"

using namespace AppNmsp;
using namespace DirectX;


int main(int argc, char** argv) 
{
	ZoneScopedN("Application");

	RRenderRandomizer renderRandomizer = RRenderRandomizer();
	renderRandomizer.Initialize();
	auto toto = RRenderRandomizer::s_GetLocalDesyncDistribution<float>();

	Camera camera;
	CameraFLT& cameraData = camera.GetData();
	cameraData.AspectRatio = 16.f / 9.f;
	cameraData.ImageSizeFromWidth(400);

	camera.Initialize();

	HList world;
	world.Add(std::make_shared<HSphere>(XMFLOAT3{ 1.f, 0.f, 0.f }, 0.5f));
	world.Add(std::make_shared<HSphere>(XMFLOAT3{ 1.f, 0.f, -100.5f }, 100.f));


	Picture resultBuffer(cameraData.ImageSize, TEXT("Result.ppm"));
	{
		ZoneScopedN("Draw");
		camera.Render(&world, &resultBuffer);
	}

	{
		ZoneScopedN("Write to disk");
		resultBuffer.WriteToDisk(true);
	}
	char r;
	std::cin >> r;

	return 0;
}