#pragma once

#include<cassert>
#include <vector>
#include <bitset>
#include <string>
#include <queue>

#include "Core/GameObject.h"
#include "ComponentManager.hpp"

namespace Core
{
	class GameObjectManager
	{
	private:
		std::vector<GameObject> _gameObjects;
		std::queue<size_t> _availableIndices;
		size_t _count;
	public:
		GameObjectManager(size_t initialCapacity = 65536);
		GameObject* CreateGameObject(const std::string& name, Transform* parent = nullptr);
		void DestroyGameObject(GameObject* gameObject);
	};


}