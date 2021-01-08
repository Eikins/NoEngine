#pragma once

#include "Core/Transform.h"
#include "Core/GameObject.h"

namespace Core
{
	class Component
	{
	private:
		GameObject* _gameObject = nullptr;
		Transform* _transform = nullptr;
	protected:
		Component(GameObject* gameObject) : _gameObject(gameObject), _transform(gameObject->GetTransform()) {}
	public:
		Transform* GetTransform();
		GameObject* GetGameObject();
	};
}