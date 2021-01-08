#pragma once

#ifdef NoEngine_Editor
	#include "imgui.h"

	#include "Core/Assets/Scene.h"
	#include "Core/Transform.h"
	#include "Core/GameObject.h"

	namespace Editor
	{
		class SceneEditors
		{
		private:
			static void DrawSceneNode(Core::Transform* transform, Core::GameObject** selectedObject);
		public:
			static void DrawSceneHierarchy(std::vector<Core::Transform*>& roots, Core::GameObject** selectedObject);
		};
	}
#endif