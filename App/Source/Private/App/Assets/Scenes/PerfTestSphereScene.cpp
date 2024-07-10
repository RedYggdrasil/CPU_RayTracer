#include "App/Assets/Scenes/PerfTestSphereScene.h"

#include "App/Tools/RRenderRandomizer.h"
#include "App/Maths/RMathRand.h"
#include "App/Assets/Materials/LambertianMat.h"
#include "App/Assets/Materials/DielectricMat.h"
#include "App/Assets/Materials/MetalMat.h"

#include<array>
#include <format>


using namespace AppNmsp;
using namespace DirectX;

constexpr std::array<float, 441> fixedDistrib_0_1_441 =
{
    0.23017f, 0.430726f, 0.0428606f, 0.405096f, 0.344292f, 0.18773f, 0.363773f, 0.300879f, 0.160596f, 0.711467f, 0.994334f, 0.223306f, 0.94921f, 0.890336f, 0.425637f, 0.275804f, 0.70876f, 0.762001f, 0.485296f, 0.0375629f, 0.670875f, 0.326579f, 0.167507f, 0.97653f, 0.0759986f, 0.91605f, 0.768234f, 0.28512f, 0.351102f, 0.633664f, 0.665334f, 0.0604363f, 0.465294f, 0.962516f, 0.111785f, 0.299006f, 0.18595f, 0.298148f, 0.282424f, 0.239149f, 0.0374921f, 0.972659f, 0.428701f, 0.765897f, 0.494275f, 0.343752f, 0.569859f, 0.81162f, 0.74878f, 0.645661f, 0.339252f, 0.324429f, 0.717367f, 0.189324f, 0.495047f, 0.207638f, 0.248435f, 0.593703f, 0.629112f, 0.663052f, 0.918122f, 0.378749f, 0.570984f, 0.683083f, 0.719138f, 0.658942f, 0.0968854f, 0.450173f, 0.66189f, 0.156918f, 0.626607f, 0.655332f, 0.608782f, 0.405896f, 0.430862f, 0.944228f, 0.303996f, 0.228825f, 0.570015f, 0.675548f, 0.300292f, 0.867694f, 0.240967f, 0.814367f, 0.755428f, 0.291962f, 0.57824f, 0.220684f, 0.287742f, 0.844045f, 0.83499f, 0.650831f, 0.539974f, 0.403874f, 0.725133f, 0.436687f, 0.272459f, 0.358582f, 0.801085f, 0.749519f, 0.939079f, 0.305698f, 0.887915f, 0.81891f, 0.385953f, 0.625943f, 0.822915f, 0.915324f, 0.307159f, 0.240735f, 0.905322f, 0.79873f, 0.832278f, 0.316541f, 0.0104445f, 0.83811f, 0.186191f, 0.746408f, 0.650096f, 0.927356f, 0.588735f, 0.0851281f, 0.802787f, 0.780641f, 0.953678f, 0.533175f, 0.845723f, 0.399757f, 0.487983f, 0.291185f, 0.299655f, 0.177079f, 0.251773f, 0.534084f, 0.646834f, 0.320354f, 0.456816f, 0.352561f, 0.928125f, 0.265195f, 0.832433f, 0.836501f, 0.297819f, 0.279588f, 0.226892f, 0.675231f, 0.556545f, 0.814232f, 0.127373f, 0.96551f, 0.990026f, 0.0610713f, 0.839827f, 0.275755f, 0.781935f, 0.319976f, 0.686691f, 0.620988f, 0.159515f, 0.447899f, 0.837011f, 0.532083f, 0.897543f, 0.279523f, 0.379737f, 0.21837f, 0.714722f, 0.847832f, 0.348896f, 0.258905f, 0.771085f, 0.935265f, 0.327452f, 0.165552f, 0.922822f, 0.0619609f, 0.149769f, 0.580482f, 0.133382f, 0.175922f, 0.736516f, 0.580064f, 0.471463f, 0.66155f, 0.962297f, 0.924728f, 0.209371f, 0.076411f, 0.996048f, 0.154828f, 0.802547f, 0.264796f, 0.306277f, 0.85521f, 0.546701f, 0.480586f, 0.810931f, 0.758925f, 0.32209f, 0.927224f, 0.163936f, 0.293882f, 0.402884f, 0.889257f, 0.402267f, 0.287434f, 0.444341f, 0.834781f, 0.08462f, 0.357984f, 0.936438f, 0.575204f, 0.758677f, 0.700707f, 0.174084f, 0.533521f, 0.262659f, 0.433819f, 0.728524f, 0.204875f, 0.7969f, 0.77062f, 0.826374f, 0.822591f, 0.106695f, 0.835579f, 0.832208f, 0.384584f, 0.655695f, 0.308874f, 0.0157862f, 0.373502f, 0.858773f, 0.380503f, 0.950577f, 0.767162f, 0.198059f, 0.253958f, 0.892803f, 0.657614f, 0.497996f, 0.195906f, 0.294163f, 0.402301f, 0.332885f, 0.392134f, 0.925726f, 0.249896f, 0.317765f, 0.196131f, 0.907845f, 0.519564f, 0.67317f, 0.765586f, 0.836167f, 0.704092f, 0.432495f, 0.595998f, 0.278605f, 0.390007f, 0.913572f, 0.489152f, 0.557729f, 0.0907281f, 0.0247514f, 0.517566f, 0.934239f, 0.977099f, 0.123435f, 0.679928f, 0.402736f, 0.165741f, 0.199713f, 0.83512f, 0.860868f, 0.583342f, 0.101442f, 0.0646079f, 0.876534f, 0.725407f, 0.15383f, 0.741348f, 0.678172f, 0.229104f, 0.900008f, 0.995623f, 0.277276f, 0.797005f, 0.716978f, 0.784645f, 0.493118f, 0.299407f, 0.634921f, 0.477164f, 0.108319f, 0.242363f, 0.203235f, 0.332991f, 0.205145f, 0.0214095f, 0.952256f, 0.225066f, 0.696051f, 0.792414f, 0.23519f, 0.583668f, 0.398449f, 0.534801f, 0.728446f, 0.478215f, 0.0390518f, 0.526551f, 0.912929f, 0.703743f, 0.968865f, 0.229612f, 0.982302f, 0.286259f, 0.269685f, 0.150844f, 0.0136145f, 0.892759f, 0.0275078f, 0.863485f, 0.31069f, 0.895212f, 0.409354f, 0.399943f, 0.808477f, 0.618511f, 0.995178f, 0.165104f, 0.798662f, 0.599347f, 0.334316f, 0.866206f, 0.387964f, 0.258554f, 0.603682f, 0.59415f, 0.685507f, 0.963833f, 0.865733f, 0.812957f, 0.280771f, 0.638919f, 0.565065f, 0.486673f, 0.148442f, 0.0642018f, 0.643133f, 0.9087f, 0.387894f, 0.293163f, 0.561397f, 0.182643f, 0.244268f, 0.351174f, 0.564352f, 0.82671f, 0.833618f, 0.599488f, 0.171393f, 0.666436f, 0.941584f, 0.0621018f, 0.516602f, 0.113621f, 0.212549f, 0.251815f, 0.337699f, 0.484697f, 0.581104f, 0.697166f, 0.00864323f, 0.923042f, 0.15477f, 0.689291f, 0.933653f, 0.835476f, 0.327607f, 0.662003f, 0.262612f, 0.537154f, 0.0333454f, 0.205139f, 0.00279186f, 0.0288124f, 0.548766f, 0.369426f, 0.941459f, 0.276663f, 0.589142f, 0.247281f, 0.903293f, 0.911536f, 0.299071f, 0.578619f, 0.154334f, 0.0659879f, 0.740278f, 0.8965f, 0.990821f, 0.985953f, 0.0266262f, 0.820635f, 0.489391f, 0.600557f, 0.314408f, 0.878521f, 0.00432947f, 0.916277f, 0.707184f, 0.708963f, 0.279678f, 0.497613f, 0.469814f, 0.730596f, 0.176659f, 0.469304f, 0.940969f, 0.00672162f, 0.816207f, 0.816951f, 0.24976f, 0.382627f, 0.28564f, 0.72583f, 0.253844f, 0.790569f, 0.237686f, 0.400882f, 0.116649f, 0.828052f, 0.943111f, 0.0266392f, 0.5329f, 0.878604f, 0.674726f, 0.969033f, 0.887193f
};

PerfTestSphereScene::PerfTestSphereScene()
    :Scene()
{
    //static thread_local RRenderRandomizer::LocalRealDistribution<float> localDistrib = RRenderRandomizer::s_GetLocalDesyncDistribution<float>(0.0f, 1.0f);
    //static thread_local LocalVectorDistribution unitVecDistrib = LocalVectorDistribution::CreateDesyncRangedDistribution(0.f, 1.f);

    CameraFLT& cameraData = m_camera.GetData();

    cameraData.AspectRatio = 16.f / 9.f;
    cameraData.ImageSizeFromWidth(720);//1200//400// (2160);
    cameraData.SamplesPerPixel = 10; //500//50
    cameraData.MaxDepth = 50;

    cameraData.vFov = 20;
    cameraData.PosLookFrom = XMFLOAT3(-3.f, 13.f, 2.f);
    cameraData.PosLookAt = XMFLOAT3(0.f, 0.f, 0.f);
    cameraData.UpVec = XMFLOAT3(0.f, 0.f, 1.f);


    cameraData.DefocusAngle = 0.6f;
    cameraData.FocusDistance = 10.f;

    m_camera.Initialize();

    std::shared_ptr<LambertianMat>	materialGround = AddMaterial<LambertianMat>(TEXT("MaterialGround"), XMFLOAT3{ 0.5f, 0.5f, 0.5f });
    m_world.Add(std::make_shared<HSphere>(XMFLOAT3{ 0.0f,  0.0f,  -1000.f }, 1000.0f, materialGround, HittableDepthType::Container0, RefractionIndex::Air));

    size_t currentIndex = std::numeric_limits<size_t>::max();
    constexpr size_t arrayCount = fixedDistrib_0_1_441.size();
    auto nextIndex = [&arrayCount, &currentIndex]() -> size_t {return (++currentIndex)% arrayCount; };

    auto GetFloat = [&nextIndex]() -> float {return fixedDistrib_0_1_441[nextIndex()]; };
    
    auto GetColorVector3 = [&nextIndex]() -> XMVECTOR 
        { 
            float r = fixedDistrib_0_1_441[nextIndex()];
            float g = fixedDistrib_0_1_441[nextIndex()];
            float b = fixedDistrib_0_1_441[nextIndex()];
            return XMVECTOR{ r, g , b, 0.f }; 
        };

    const XMVECTOR lConstPoint = XMVECTOR{ 0.f, 4.f, 0.2f };
    for (int32_t a = -11; a < 11; a++) 
    {
        for (int32_t b = -11; b < 11; b++) 
        {
            float MaterialPercent = GetFloat();
            float fltforward = GetFloat();
            float fltRight = GetFloat();
            XMVECTOR lCenter = XMVECTOR{ (b + 0.9f * fltforward) * -1.f, a + 0.9f * fltRight,  0.2f };

            XMVECTOR optVec3_01 = GetColorVector3();
            XMVECTOR optVec3_02 = GetColorVector3();
            float optFLT_01 = GetFloat();
            if (XMVectorGetX(XMVector3Length(lCenter - lConstPoint)) > 0.9f)
            {
                std::shared_ptr<Material> sphereMat;
                XMFLOAT3 centerFLT3;
                XMStoreFloat3(&centerFLT3, lCenter);

                if (MaterialPercent < 0.8f)
                {
                    // diffuse
                    XMFLOAT3 albedoColor;
                    XMStoreFloat3(&albedoColor, optVec3_01 * optVec3_02);
                    sphereMat = AddMaterial<LambertianMat>(std::format("MaterialDiffuse_{}_{}", a, b), albedoColor);
                }
                else if (MaterialPercent < 0.95f) {
                    // metal
                    XMFLOAT3 albedoColor;
                    XMStoreFloat3(&albedoColor, (optVec3_01 * 0.5f) + XMVECTOR{ 0.5f, 0.5f, 0.5f, 0.f });
                    float fuzz = optFLT_01 * 0.5f;
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
    m_world.Add(std::make_shared<HSphere>(XMFLOAT3{ 0.f, 0.f, 1.f }, 1.f, material1, HittableDepthType::Container0, RefractionIndex::Air));

    std::shared_ptr<LambertianMat> material2 = AddMaterial<LambertianMat>(TEXT("MaterialLambertian2"), XMFLOAT3{ 0.4f, 0.2f, 0.1f });
    m_world.Add(std::make_shared<HSphere>(XMFLOAT3{ 0.f, -4.f, 1.f }, 1.f, material2, HittableDepthType::Container0, RefractionIndex::Air));

    std::shared_ptr<MetalMat> material3 = AddMaterial<MetalMat>(TEXT("MaterialMetal3"), XMFLOAT3{ 0.7f, 0.6f, 0.5f }, 0.0f);
    m_world.Add(std::make_shared<HSphere>(XMFLOAT3{ 0.f, 4.f, 1.f }, 1.f, material3, HittableDepthType::Container0, RefractionIndex::Air));
}