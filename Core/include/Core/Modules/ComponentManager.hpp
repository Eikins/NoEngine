#pragma once
#include<cassert>

#include <type_traits>
#include <utility>
#include <unordered_map>

#include "ComponentContainer.hpp"

namespace Core
{
	class ComponentManager
	{
	private:
		std::unordered_map<const char*, ComponentType> _componentTypes;
		std::unordered_map<const char*, AbstractComponentContainer*> _componentContainers;
		std::unordered_map<ComponentType, AbstractComponentContainer*> _componentContainersFromType;
		ComponentType _nextAvailableType;

		template<typename T>
		ComponentContainer<T>* GetContainer()
		{
			const char* typeName = typeid(T).name();
			assert(_componentTypes.find(typeName) != _componentTypes.end() && "Component not registered.");

			return static_cast<ComponentContainer<T>*>(_componentContainers[typeName]);
		}
	public:
		template<typename T>
		void RegisterComponent()
		{
			auto typeName = typeid(T).name();
			assert(_componentTypes.find(typeName) == _componentTypes.end() && "Component already registered.");

			_componentTypes.insert({ typeName, _nextAvailableType });
			auto container = new ComponentContainer<T>();
			_componentContainers.insert({ typeName, container });
			_componentContainersFromType.insert({ _nextAvailableType, container });
			_nextAvailableType++;
		}

		template<typename T>
		ComponentType GetType()
		{
			const char* typeName = typeid(T).name();
			assert(_componentTypes.find(typeName) != _componentTypes.end() && "Component not registered.");

			return _componentTypes[typeName];
		}

		template<typename T>
		T& AddComponent(GameObject* gameObject)
		{
			return GetContainer<T>()->Add(gameObject);
		}

		template<typename T>
		void RemoveComponent(GameObject* gameObject)
		{
			GetContainer<T>()->RemoveUnsafe(gameObject);
		}

		template<typename T>
		T* GetComponent(GameObject* gameObject)
		{
			return GetContainer<T>()->Get(gameObject);
		}

		void* GetComponentRaw(GameObject* gameObject, ComponentType type);

		template<typename T>
		std::vector<T>& GetAllComponents()
		{
			return GetContainer<T>()->Components();
		}

		void GameObjectRemoved(GameObject* gameObject)
		{
			// Remove all components safely (i.e. by checking if the component exist)
			for (auto const& pair : _componentContainers)
			{
				auto const& container = pair.second;
				container->RemoveSafe(gameObject);
			}
		}
	};
}