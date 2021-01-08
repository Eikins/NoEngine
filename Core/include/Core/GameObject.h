#pragma once

#include <bitset>

#include "Transform.h"

#ifdef NoEngine_Editor
namespace Editor
{
	class GameObjectEditor;
	class SceneEditors;
}
#endif

namespace Core
{
	typedef int ComponentType;
	const ComponentType MAX_COMPONENT_TYPES = 32;
	typedef std::bitset<MAX_COMPONENT_TYPES> Signature;

	class GameObject
	{
#ifdef NoEngine_Editor
		friend class Editor::GameObjectEditor;
		friend class Editor::SceneEditors;
#endif
		friend class SystemManager;
		friend class GameObjectManager;
		friend class GameManager;
	private:
		Signature _signature;
		std::string _name = "";
		Transform _transform;

		GameObject() {}
		void Init(std::string name, Transform* parent = nullptr);
	public:
		Transform* GetTransform();
	};
}