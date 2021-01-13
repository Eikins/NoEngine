#pragma once

#include <cassert>

#include <type_traits>
#include <utility>
#include <unordered_map>

#include "Core/Components/Component.h"

namespace Core
{
	class AbstractComponentContainer
	{
	public:
		virtual void RemoveSafe(GameObject* gameObject) = 0;
		virtual void* GetRaw(GameObject* gameObject) = 0;
	};

	template<typename T>
	class ComponentContainer : public AbstractComponentContainer
	{
		// Compile-time sanity check, a ComponentContainer is only available for Component derived classes
		static_assert(std::is_base_of<Component, T>::value, "The type is not a Component");

	private:
		// Used to store in a contiguous way the components
		std::vector<T> _elements;
		std::unordered_map<GameObject*, size_t> _gameObjectToIndex;
		std::unordered_map<size_t, GameObject*> _indexToGameObject;

	public:
		ComponentContainer(size_t initialCapacity = 65536)
		{
			_elements.reserve(initialCapacity);
			_gameObjectToIndex.reserve(initialCapacity);
			_indexToGameObject.reserve(initialCapacity);
		}

		// Forward arguments when creating a component
		T& Add(GameObject* gameObject)
		{
			assert(_gameObjectToIndex.find(gameObject) == _gameObjectToIndex.end() && "Component added to same game object more than once.");

			size_t index = _elements.size();
			_gameObjectToIndex[gameObject] = index;
			_indexToGameObject[index] = gameObject;
			_elements.push_back(T(gameObject));
			return _elements[index];
		}

		void RemoveUnsafe(GameObject* gameObject)
		{
			size_t index = _gameObjectToIndex[gameObject];
			size_t last = _elements.size() - 1;

			// First, swap the objects, this helps keeping contiguous data in O(1)
			_elements[index] = _elements[last];
			auto lastObject = _indexToGameObject[last];
			_gameObjectToIndex[lastObject] = index;
			_indexToGameObject[index] = lastObject;

			_gameObjectToIndex.erase(gameObject);
			_indexToGameObject.erase(last);
			_elements.pop_back();
		}

		virtual void RemoveSafe(GameObject* gameObject) override
		{
			if (_gameObjectToIndex.find(gameObject) != _gameObjectToIndex.end())
			{
				RemoveUnsafe(gameObject);
			}
		}

		virtual void* GetRaw(GameObject* gameObject) override
		{
			auto& it = _gameObjectToIndex.find(gameObject);
			if (it == _gameObjectToIndex.end()) return nullptr;
			else return &_elements[it->second];
		}

		// Get the component of a given gameobject
		T* Get(GameObject* gameObject)
		{
			auto& it = _gameObjectToIndex.find(gameObject);
			if (it == _gameObjectToIndex.end())
			{
				return nullptr;
			}
			else
			{
				return &_elements[it->second];
			}
		}

		inline std::vector<T>& Components() { return _elements; }
	};
}
