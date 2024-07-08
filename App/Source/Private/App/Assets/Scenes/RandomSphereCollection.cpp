#include "App/Assets/Scenes/RandomSphereCollection.h"
#include "App/Tools/RRenderRandomizer.h"
#include "App/Maths/RMathRand.h"
#include "App/Assets/Materials/LambertianMat.h"
#include "App/Assets/Materials/DielectricMat.h"
#include "App/Assets/Materials/MetalMat.h"

#include <format>


using namespace AppNmsp;
using namespace DirectX;


RandomSphereCollection::RandomSphereCollection()
	:Scene()
{
    static thread_local RRenderRandomizer::LocalRealDistribution<float> localDistrib = RRenderRandomizer::s_GetLocalDesyncDistribution<float>(0.0f, 1.0f);
    static thread_local LocalVectorDistribution unitVecDistrib = LocalVectorDistribution::CreateDesyncRangedDistribution(0.f, 1.f);

	CameraFLT& cameraData = m_camera.GetData();

	cameraData.AspectRatio = 16.f / 9.f;
	cameraData.ImageSizeFromWidth(1200);//400// (2160);
	cameraData.SamplesPerPixel = 500; //50
	cameraData.MaxDepth = 50;

	cameraData.vFov = 20;
	cameraData.PosLookFrom	= XMFLOAT3( -3.f, 13.f,  2.f);
	cameraData.PosLookAt	= XMFLOAT3(  0.f,  0.f,  0.f);
	cameraData.UpVec		= XMFLOAT3(  0.f,  0.f,  1.f);


	cameraData.DefocusAngle = 0.6f;
	cameraData.FocusDistance = 10.f;

	m_camera.Initialize();

    std::shared_ptr<LambertianMat>	materialGround = AddMaterial<LambertianMat>(TEXT("MaterialGround"), XMFLOAT3{ 0.5f, 0.5f, 0.5f });
    m_world.Add(std::make_shared<HSphere>(XMFLOAT3{ 0.0f,  0.0f,  -1000.f }, 1000.0f, materialGround, HittableDepthType::Container0, RefractionIndex::Air));

    const XMVECTOR lConstPoint = XMVECTOR{ 0.f, 4.f, 0.2f };
    for (int32_t a = -11; a < 11; a++) {
        for (int32_t b = -11; b < 11; b++) {
            float MaterialPercent = localDistrib();
            XMVECTOR lCenter = XMVECTOR{ (b + 0.9f * localDistrib()) * -1.f, a + 0.9f * localDistrib(),  0.2f };

            if (XMVectorGetX(XMVector3Length(lCenter - lConstPoint)) > 0.9f) 
            {
                std::shared_ptr<Material> sphereMat;
                XMFLOAT3 centerFLT3;
                XMStoreFloat3(&centerFLT3, lCenter);

                if (MaterialPercent < 0.8f) 
                {
                    // diffuse
                    XMFLOAT3 albedoColor;
                    XMStoreFloat3(&albedoColor, unitVecDistrib.GetVEC3() * unitVecDistrib.GetVEC3());
                    sphereMat = AddMaterial<LambertianMat>(std::format("MaterialDiffuse_{}_{}", a, b), albedoColor);
                }
                else if (MaterialPercent < 0.95f) {
                    // metal
                    XMFLOAT3 albedoColor;
                    XMStoreFloat3(&albedoColor, unitVecDistrib.GetVEC3(0.5f, 1.f));
                    float fuzz = localDistrib() * 0.5f;
                    sphereMat = AddMaterial<MetalMat>(std::format("MaterialMetal_{}_{}", a, b), albedoColor, fuzz);
                }
                else {
                    // glass
                    sphereMat = AddMaterial<DielectricMat>(std::format("MaterialDielectric_{}_{}", a, b), RefractionIndex::Glass);
                }
                m_world.Add(std::make_shared<HSphere>(centerFLT3, 0.2f, sphereMat, HittableDepthType::Container0, RefractionIndex::Air));
            }
        }
    }

    std::shared_ptr<DielectricMat> material1 = AddMaterial<DielectricMat>(TEXT("MaterialDielectric1"), RefractionIndex::Glass);
    m_world.Add(std::make_shared<HSphere>(XMFLOAT3{0.f, 0.f, 1.f}, 1.f, material1, HittableDepthType::Container0, RefractionIndex::Air));

    std::shared_ptr<LambertianMat> material2 = AddMaterial<LambertianMat>(TEXT("MaterialLambertian2"), XMFLOAT3{ 0.4f, 0.2f, 0.1f });
    m_world.Add(std::make_shared<HSphere>(XMFLOAT3{ 0.f, -4.f, 1.f }, 1.f, material2, HittableDepthType::Container0, RefractionIndex::Air));

    std::shared_ptr<MetalMat> material3 = AddMaterial<MetalMat>(TEXT("MaterialMetal3"), XMFLOAT3{ 0.7f, 0.6f, 0.5f }, 0.0f);
    m_world.Add(std::make_shared<HSphere>(XMFLOAT3{ 0.f, 4.f, 1.f }, 1.f, material3, HittableDepthType::Container0, RefractionIndex::Air));
}