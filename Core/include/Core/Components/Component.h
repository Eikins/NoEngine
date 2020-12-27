#pragma once

#include "../Transform.h"

namespace Core
{
	class Component
	{
	private:
		// GameObject* _gameObject;
	public:
		Transform* _transform;
		Transform* GetTransform();
		// GameObject* GetGameObject();
	};
}