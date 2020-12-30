#pragma once

#include "../Transform.h"

namespace Core
{
	enum class ComponentType : uint8_t
	{
		Camera = 0,
		Renderer = 1
	};

	class Component
	{
	private:
		// GameObject* _gameObject;
		ComponentType _type;
	protected:
		Component(ComponentType type) : _type(type) {}
	public:
		Transform* _transform = nullptr;
		Transform* GetTransform();
		// GameObject* GetGameObject();

		ComponentType GetType();

		operator int() const;
	};
}