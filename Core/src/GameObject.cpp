#include "Core/GameObject.h"

#include "Core/Transform.h"

namespace Core
{

	void GameObject::Init()
	{
		_transform = Transform(this);
	}

	Transform* GameObject::GetTransform()
	{
		return &_transform;
	}

	bool GameObject::IsActive() const
	{
		if (_transform._parent == nullptr)
		{
			return IsActiveSelf();
		}
		else
		{
			return IsActiveSelf() && _transform._parent->_gameObject->IsActive();
		}
	}
}