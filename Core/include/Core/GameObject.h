#pragma once

#include "Transform.h"
#include "Components/Component.h"

#ifdef NoEngine_Editor
namespace Editor
{
	class GameObjectEditor;
	class SceneEditors;
}
#endif

namespace Core
{
	class Scene;

	class GameObject
	{
#ifdef NoEngine_Editor
		friend class Editor::GameObjectEditor;
		friend class Editor::SceneEditors;
#endif
		friend class Scene;
	private:
		Scene* _scene = nullptr;
		uint32_t sceneIndex = 0;
		std::string _name = "";
		Transform _transform;
		std::vector<Component*> _components;

		GameObject() {}
		//GameObject(std::string name, Transform* parent = nullptr) : _name(name), _transform(this) {}
	public:		
		Component& AddComponent(ComponentType componentType);

		Transform& GetTransform();
	};
}