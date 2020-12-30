#include "Core/Components/Component.h"

namespace Core
{
	Transform* Component::GetTransform() { return _transform; }
	ComponentType Component::GetType() { return _type; }
}