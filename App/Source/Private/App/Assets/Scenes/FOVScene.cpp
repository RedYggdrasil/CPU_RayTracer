#include "App/Assets/Scenes/FOVScene.h"
#include "App/Assets/Materials/LambertianMat.h"

using namespace AppNmsp;
using namespace DirectX;

FOVScene::FOVScene()
:Scene()
{
	CameraFLT& cameraData = m_camera.GetData();
	cameraData.AspectRatio = 16.f / 9.f;
	cameraData.ImageSizeFromWidth(400);// (2160);
	cameraData.SamplesPerPixel = 100;
	cameraData.SamplesPerPixel = 100;

	cameraData.vFov = 90;

	m_camera.Initialize();

	std::shared_ptr<LambertianMat>	materialLeft	= AddMaterial<LambertianMat>(TEXT("MaterialLeft") , XMFLOAT3{ 0.0f, 0.0f, 1.0f });
	std::shared_ptr<LambertianMat>	materialRight	= AddMaterial<LambertianMat>(TEXT("MaterialRight"), XMFLOAT3{ 1.0f, 0.0f, 0.0f });

	float radius = cos(XM_PI / 4);

	m_world.Add(std::make_shared<HSphere>(XMFLOAT3{ 1.f,  -radius,  0.f }, radius, materialLeft , HittableDepthType::Container0, RefractionIndex::Air));
	m_world.Add(std::make_shared<HSphere>(XMFLOAT3{ 1.f,  +radius,  0.f }, radius, materialRight, HittableDepthType::Container0, RefractionIndex::Air));
}
