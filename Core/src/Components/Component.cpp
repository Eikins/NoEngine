#include "Core/Components/Component.h"

namespace Core
{
	Transform* Component::GetTransform() { return _transform; }
	GameObject* Component::GetGameObject() { return _gameObject; }
	ComponentType Component::GetType() { return _type; }
}