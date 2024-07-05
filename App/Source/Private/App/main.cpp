#include "App/pch.h"

#include "App/Assets/Materials/DielectricMat.h"
#include "App/Assets/Materials/LambertianMat.h"
#include "App/Assets/Materials/MetalMat.h"
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

#include <chrono>

using namespace AppNmsp;
using namespace DirectX;


int main(int argc, char** argv) 
{
	ZoneScopedN("Application");

	RRenderRandomizer renderRandomizer = RRenderRandomizer();
	renderRandomizer.Initialize();

	Camera camera;
	CameraFLT& cameraData = camera.GetData();
	cameraData.AspectRatio = 16.f / 9.f;
	cameraData.ImageSizeFromWidth(400);// (2160);
	cameraData.SamplesPerPixel = 100;
	cameraData.SamplesPerPixel = 50;

	camera.Initialize();

	HList world;

	std::shared_ptr<Material> materialGround	= std::make_shared<LambertianMat>	(XMFLOAT3{ 0.8f, 0.8f, 0.0f });
	std::shared_ptr<Material> materialCenter	= std::make_shared<LambertianMat>	(XMFLOAT3{ 0.1f, 0.2f, 0.5f });
	std::shared_ptr<Material> materialLeft		= std::make_shared<DielectricMat>	(1.00f / 1.33f);
	std::shared_ptr<Material> materialRight		= std::make_shared<MetalMat>		(XMFLOAT3{ 0.8f, 0.6f, 0.2f }, 1.0f);


	world.Add(std::make_shared<HSphere>(XMFLOAT3{ 1.0f,  0.0f,  -100.5f }, 100.0f, materialGround,	HittableDepthType::Container0, RefractionIndex::Air));
	world.Add(std::make_shared<HSphere>(XMFLOAT3{ 1.2f,  0.0f,   000.0f }, 000.5f, materialCenter,	HittableDepthType::Container0, RefractionIndex::Air));
	world.Add(std::make_shared<HSphere>(XMFLOAT3{ 1.0f, -1.0f,   000.0f }, 000.5f, materialLeft,	HittableDepthType::Container0, RefractionIndex::Air));
	world.Add(std::make_shared<HSphere>(XMFLOAT3{ 1.0f,  1.0f,   000.0f }, 000.5f, materialRight,	HittableDepthType::Container0, RefractionIndex::Air));

	std::chrono::steady_clock::time_point startTime = std::chrono::high_resolution_clock::now();
	std::chrono::steady_clock::time_point endRenderTime;
	std::chrono::steady_clock::time_point endWriteTime;
	Picture resultBuffer(cameraData.ImageSize, TEXT("Result.ppm"));
	{
		ZoneScopedN("Draw");
		camera.Render(&world, &resultBuffer);
		endRenderTime = std::chrono::high_resolution_clock::now();
	}

	{
		ZoneScopedN("Write to disk");
		resultBuffer.WriteToDisk(true);
		endWriteTime = std::chrono::high_resolution_clock::now();
	}
	auto durationRender = std::chrono::duration_cast<std::chrono::microseconds>(endRenderTime - startTime);
	auto durationRenderAndWrite = std::chrono::duration_cast<std::chrono::microseconds>(endWriteTime - startTime);
	std::cout << "\rRendering took : " << (double)durationRender.count() / 1000000.0 << " seconds (" 
		<< (double)durationRenderAndWrite.count() / 1000000.0 <<" if accounting for write time)" << std::endl;

	char r;
	std::cin >> r;

	return 0;
}