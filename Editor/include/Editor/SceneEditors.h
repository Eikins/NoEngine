#pragma once

#ifdef NoEngine_Editor
	#include "imgui.h"

	#include "Core/Scene.h"

	namespace Editor
	{
		class SceneEditors
		{
		private:
			static void DrawSceneNode(Core::Transform* transform, int* selectionSceneIndex);
		public:
			static void DrawSceneHierarchy(Core::Scene& scene, int* selectionSceneIndex);
		};
	}
#endif