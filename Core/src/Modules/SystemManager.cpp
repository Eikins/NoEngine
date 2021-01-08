#pragma once

#include "Core/Modules/SystemManager.hpp"

namespace Core
{
	void SystemManager::OnGameObjectRemoved(GameObject* gameObject)
	{
		for (auto const& pair : _systems)
		{
			auto const& system = pair.second;
			system->_gameObjects.erase(gameObject);
		}
	}

	void SystemManager::OnGameObjectSignatureChanged(GameObject* gameObject)
	{
		auto& signature = gameObject->_signature;
		for (auto const& pair : _systems)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = _signatures[type];

			if ((signature & systemSignature) == systemSignature)
			{
				system->_gameObjects.insert(gameObject);
			}
			else
			{
				system->_gameObjects.erase(gameObject);
			}
		}
	}
}