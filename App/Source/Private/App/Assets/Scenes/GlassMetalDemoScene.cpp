#include "App/Assets/Scenes/GlassMetalDemoScene.h"
#include "App/Assets/Materials/LambertianMat.h"
#include "App/Assets/Materials/DielectricMat.h"
#include "App/Assets/Materials/MetalMat.h"

using namespace AppNmsp;
using namespace DirectX;

AppNmsp::GlassMetalDemoScene::GlassMetalDemoScene()
:Scene()
{
	CameraFLT& cameraData = m_camera.GetData();
	cameraData.AspectRatio = 16.f / 9.f;
	cameraData.ImageSizeFromWidth(400);// (2160);
	cameraData.SamplesPerPixel = 100;
	cameraData.SamplesPerPixel = 100;

	m_camera.Initialize();

	std::shared_ptr<LambertianMat>	materialGround		= AddMaterial<LambertianMat>	(TEXT("MaterialGround")	, XMFLOAT3{ 0.8f, 0.8f, 0.0f });
	std::shared_ptr<LambertianMat>	materialCenter		= AddMaterial<LambertianMat>	(TEXT("MaterialCenter")	, XMFLOAT3{ 0.1f, 0.2f, 0.5f });
	std::shared_ptr<DielectricMat>	materialLeft		= AddMaterial<DielectricMat>	(TEXT("MaterialLeft")	, RefractionIndex::Glass);
	std::shared_ptr<DielectricMat>	materialBubble		= AddMaterial<DielectricMat>	(TEXT("MaterialBubble") , RefractionIndex::Air);
	std::shared_ptr<MetalMat>		materialRight		= AddMaterial<MetalMat>			(TEXT("MaterialRight")	, XMFLOAT3{ 0.8f, 0.6f, 0.2f }, 1.0f);


	m_world.Add(std::make_shared<HSphere>(XMFLOAT3{ 1.0f,  0.0f,  -100.5f }, 100.0f, materialGround, HittableDepthType::Container0, RefractionIndex::Air));
	m_world.Add(std::make_shared<HSphere>(XMFLOAT3{ 1.2f,  0.0f,   000.0f }, 000.5f, materialCenter, HittableDepthType::Container0, RefractionIndex::Air));
	m_world.Add(std::make_shared<HSphere>(XMFLOAT3{ 1.0f, -1.0f,   000.0f }, 000.5f, materialLeft, HittableDepthType::Container0, RefractionIndex::Air));
	m_world.Add(std::make_shared<HSphere>(XMFLOAT3{ 1.0f, -1.0f,   000.0f }, 000.4f, materialBubble, HittableDepthType::Container0, RefractionIndex::Glass));
	m_world.Add(std::make_shared<HSphere>(XMFLOAT3{ 1.0f,  1.0f,   000.0f }, 000.5f, materialRight, HittableDepthType::Container0, RefractionIndex::Air));
}
