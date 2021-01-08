#include "Core/GameObject.h"

#include "Core/Transform.h"

namespace Core
{

	void GameObject::Init(std::string name, Transform* parent)
	{
		_name = name;
		_transform = Transform(this, parent);
	}

	Transform* GameObject::GetTransform()
	{
		return &_transform;
	}
}