#pragma once

#include "App/pch.h"
#include "App/Assets/Materials/Material.h"
#include "App/Tools/RCollection.h"
#include "App/Maths/Camera.h"
#include "App/Hittables/HList.h"
#include "App/Hittables/HSphere.h"

#include <unordered_map>

namespace AppNmsp
{
	class Picture;
	class Scene
	{
	protected:
		Camera m_camera;
		RUnorderedStringMap<std::shared_ptr<Material>> m_materials;
		HList m_world;
	public:
		Camera& GetCamera() { return m_camera; }
		const Camera& GetCamera() const { return m_camera; }
		HList& GetWorld() { return m_world; }
		const HList& GetWorld() const { return m_world; }
	public:
		void Render(AppNmsp::Picture* InRenderTarget) const;
	protected:

		bool ExistMaterial(const std::string_view InKey) const;
		std::shared_ptr<Material> GetMaterial(const std::string_view InKey);
		/// <summary>
		/// Try adding Asset
		/// </summary>
		/// <param name="InKey">Key, add or exist</param>
		/// <param name="InAsset">Asset pointer, add or same exist</param>
		/// <returns>false if another asset is present with the given key</returns>
		bool AddMaterial(const std::string_view InKey, std::shared_ptr<Material> InAsset);

		template<class MaterialType, typename... Args>
		std::shared_ptr<MaterialType> AddMaterial(const std::string_view InKey, Args&&... args)
		{
			std::shared_ptr<MaterialType> TypedAssetPtr = std::make_shared<MaterialType>(std::forward<Args>(args)...);
			if (AddMaterial(InKey, (std::shared_ptr<MaterialType>)TypedAssetPtr))
			{
				return TypedAssetPtr;
			}
			return nullptr;
		}
	public:
		virtual ~Scene() {};
	protected:
		Scene() {};
	};
};

#define SIMPLE_SCENE(SceneClassName)\
namespace AppNmsp\
{\
	class SceneClassName : public Scene\
	{\
	public:\
		R_VIRTUAL_IMPLICIT ~SceneClassName() R_OVERRIDE_IMPLICIT {};\
		SceneClassName();\
	};\
};

#define SIMPLE_SCENE_WITH_PARENT(SceneClassName, ParentSceneClass)\
namespace AppNmsp\
{\
	class SceneClassName : public ParentSceneClass\
	{\
	public:\
		R_VIRTUAL_IMPLICIT ~SceneClassName() R_OVERRIDE_IMPLICIT {};\
		SceneClassName();\
	};\
};