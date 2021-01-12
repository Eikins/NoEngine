#pragma once

#include <vector>
#include <array>

#include "Core/Assets/Asset.h"
#include "Core/Transform.h"

#ifdef NoEngine_Editor
namespace Editor
{
	class SceneEditors;
}
#endif

namespace Core
{
	class Scene : public Asset
	{
#ifdef NoEngine_Editor
		friend class Editor::SceneEditors;
#endif
	private:
	public:
		std::vector<Transform*> rootTransforms;

		Scene(std::string name) : Asset(name) {}

		void Update();
	};
}