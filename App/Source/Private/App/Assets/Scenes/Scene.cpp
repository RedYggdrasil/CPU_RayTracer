#include "App/Assets/Scenes/Scene.h"
#include "App/SystemElement/Picture.h"

using namespace AppNmsp;
using namespace DirectX;

void Scene::Render(Picture* InRenderTarget) const
{
	m_camera.Render(&m_world, InRenderTarget);
}

bool AppNmsp::Scene::ExistMaterial(const std::string_view InKey) const
{
	auto pair = m_materials.find(InKey);
	if (pair != m_materials.end())
	{
		return pair->second.use_count() > 0;
	}
	return false;
}

std::shared_ptr<Material> AppNmsp::Scene::GetMaterial(const std::string_view InKey)
{
	auto pair = m_materials.find(InKey);
	if (pair != m_materials.end())
	{
		if (pair->second.use_count() > 0)
		{
			return pair->second;
		}
	}
	return nullptr;
}

bool AppNmsp::Scene::AddMaterial(const std::string_view InKey, std::shared_ptr<Material> InAsset)
{
	if (!InAsset)
	{
		return false;
	}
	if (ExistMaterial(InKey))
	{
		const std::shared_ptr<Material> ExistingAsset = GetMaterial(InKey);
		if (ExistingAsset != InAsset)
		{
			return false;
		}
		return true;
	}
	m_materials[std::string(InKey)] = InAsset;
	return true;
}
