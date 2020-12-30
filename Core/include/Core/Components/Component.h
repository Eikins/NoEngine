#pragma once

#include "../Transform.h"

namespace Core
{
	enum class ComponentType : uint8_t
	{
		Camera = 0,
		Renderer = 1,
		ScriptedBehaviour = 2
	};

	class GameObject;

	class Component
	{
		friend class GameObject;
	private:
		ComponentType _type;
		GameObject* _gameObject = nullptr;
		Transform* _transform = nullptr;
	protected:
		Component(ComponentType type) : _type(type) {}
	public:
		Transform* GetTransform();
		GameObject* GetGameObject();

		ComponentType GetType();
	};
}