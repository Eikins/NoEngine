#include "Core/GameObject.h"

#include "Core/Transform.h"
#include "Core/Components/Component.h"
#include "Core/Scene.h"

namespace Core
{
	Component& GameObject::AddComponent(ComponentType component)
	{
		Component& comp = _scene->CreateComponent(component);
		comp._transform = &(_transform);
		_components.push_back(&comp);
		return comp;
	}

	Transform& GameObject::GetTransform()
	{
		return _transform;
	}
}