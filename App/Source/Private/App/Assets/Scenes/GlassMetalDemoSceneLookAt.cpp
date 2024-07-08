#include "App/Assets/Scenes/GlassMetalDemoSceneLookAt.h"

using namespace AppNmsp;
using namespace DirectX;


GlassMetalDemoSceneLookAt::GlassMetalDemoSceneLookAt()
:GlassMetalDemoScene()
{
	CameraFLT& cameraData = m_camera.GetData();

	cameraData.vFov			= 20.f;

	cameraData.PosLookFrom	= XMFLOAT3(-1.f, -2.f, 2.f);
	cameraData.PosLookAt	= XMFLOAT3(1.f, 0.f, 0.f);
	cameraData.UpVec		= XMFLOAT3(0.f, 0.f, 1.f);

	m_camera.Initialize();
}