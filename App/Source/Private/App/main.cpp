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

#include "App/Assets/Scenes/Scene.h"
#include "App/Assets/Scenes/FOVScene.h"
#include "App/Assets/Scenes/GlassMetalDemoSceneLookAt.h"

#include <iostream>

#include "Tracy.hpp"
#include "TracyC.h"

#include <chrono>

using namespace AppNmsp;
using namespace DirectX;


int main(int argc, char** argv) 
{
	//Initialize app
	ZoneScopedN("Application");
	RRenderRandomizer renderRandomizer = RRenderRandomizer();
	renderRandomizer.Initialize();

	//Choose a scene
	Scene* currentScene = new GlassMetalDemoSceneLookAt();


	Picture resultBuffer(currentScene->GetCamera().GetData().GetImageSize(), TEXT("Result.ppm"));

	//Start Chronos
	std::chrono::steady_clock::time_point startTime = std::chrono::high_resolution_clock::now();
	std::chrono::steady_clock::time_point endRenderTime;
	std::chrono::steady_clock::time_point endWriteTime;

	
	//Draw the scene
	{
		ZoneScopedN("Draw");
		currentScene->Render(&resultBuffer);
		endRenderTime = std::chrono::high_resolution_clock::now();
	}

	//Write to disk
	{
		ZoneScopedN("Write to disk");
		resultBuffer.WriteToDisk(true);
		endWriteTime = std::chrono::high_resolution_clock::now();
	}

	//Stop Chronos
	auto durationRender = std::chrono::duration_cast<std::chrono::microseconds>(endRenderTime - startTime);
	auto durationRenderAndWrite = std::chrono::duration_cast<std::chrono::microseconds>(endWriteTime - startTime);

	//Clean heap
	delete(currentScene);

	//Output result and wait on exit command
	std::cout << "\rRendering took : " << (double)durationRender.count() / 1000000.0 << " seconds (" 
		<< (double)durationRenderAndWrite.count() / 1000000.0 <<" if accounting for write time)" << std::endl;
	char r;
	std::cin >> r;

	return 0;
}