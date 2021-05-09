#include "Core/Modules/ComponentManager.hpp"

namespace Core
{
	void* ComponentManager::GetComponentRaw(GameObject* gameObject, ComponentType type)
	{
		auto& it = _componentContainersFromType.find(type);
		if (it == _componentContainersFromType.end())
		{
			return nullptr;
		}
		else
		{
			return it->second->GetRaw(gameObject);
		}
	}
}