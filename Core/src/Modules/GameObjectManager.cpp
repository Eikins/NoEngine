#include "Core/Modules/GameObjectManager.h"

namespace Core
{
	GameObjectManager::GameObjectManager(size_t initialCapacity)
	{
		_gameObjects.reserve(initialCapacity);
	}

	GameObject* GameObjectManager::CreateGameObject(std::string name, Transform* parent)
	{
		size_t index;
		if (_availableIndices.empty())
		{
			index = _gameObjects.size();
			_gameObjects.push_back(GameObject());
		}
		else
		{
			index = _availableIndices.front();
			_availableIndices.pop();
			_gameObjects[index] = GameObject();
		}
		_count++;
		_gameObjects[index].Init(name, parent);
		return &_gameObjects[index];
	}

	void GameObjectManager::DestroyGameObject(GameObject* gameObject)
	{
		std::ptrdiff_t index = gameObject - &_gameObjects[0];
		_availableIndices.push(index);
		_count--;
	}
}