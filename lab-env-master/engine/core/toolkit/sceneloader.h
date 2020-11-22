#ifndef _SCENE_LOADER_H_
#define _SCENE_LOADER_H_
#include <vector>

#include "core/math/vector3D.h"
#include "render/base/entity.h"
#include "render/managers/entitymanager.h"
#include "yaml-cpp/node.h"

namespace Toolkit
{
	struct Scene
	{
		std::vector<Base::Entity*> entitys;
	};
	
	class SceneLoader
	{
	public:
		SceneLoader() = default;
		void Init();
		static SceneLoader* GetInstance();
		
	private:
		void LoadScene();
		void LoadEntities();

		Scene scene;
		Managers::EntityManager* entityManager;
		static SceneLoader* instance;
		
	};
}



#endif

