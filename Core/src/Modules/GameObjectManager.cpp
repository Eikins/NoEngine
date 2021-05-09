#include "Core/Modules/GameObjectManager.h"

namespace Core
{
	GameObjectManager::GameObjectManager(size_t initialCapacity)
	{
		_gameObjects.reserve(initialCapacity);
	}

	GameObject* GameObjectManager::CreateGameObject(const std::string& name, Transform* parent)
	{
		size_t index;
		if (_availableIndices.empty())
		{
			index = _gameObjects.size();
			_gameObjects.push_back(GameObject(name));
		}
		else
		{
			index = _availableIndices.front();
			_availableIndices.pop();
			_gameObjects[index] = GameObject(name);
		}
		_count++;
		_gameObjects[index].Init();
		_gameObjects[index].GetTransform()->SetParent(parent);
		return &_gameObjects[index];
	}

	void GameObjectManager::DestroyGameObject(GameObject* gameObject)
	{
		std::ptrdiff_t index = gameObject - &_gameObjects[0];
		_availableIndices.push(index);
		_count--;
	}
}