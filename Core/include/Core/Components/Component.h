#pragma once

#include "Core/Transform.h"
#include "Core/GameObject.h"

#ifdef NoEngine_Editor
namespace Editor
{
	class ComponentEditors;
}
#endif

namespace Core
{
	class Component
	{
#ifdef NoEngine_Editor
		friend class Editor::ComponentEditors;
#endif
	private:
		GameObject* _gameObject = nullptr;
		Transform* _transform = nullptr;
		bool _enabled = true;
	protected:
		Component(GameObject* gameObject) : _gameObject(gameObject), _transform(gameObject->GetTransform()) {}
	public:
		inline Transform* GetTransform() { return _transform; }
		inline GameObject* GetGameObject() { return _gameObject; }
		inline void SetEnabled(bool enabled) { _enabled = enabled; }
		inline bool IsEnabled() const { return _enabled; }
	};
}