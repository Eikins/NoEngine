#include "Core/Modules/GameManager.hpp"

namespace Core
{
	GameManager* GameManager::_instance = nullptr;

	GameManager* GameManager::Get()
	{
		return _instance;
	}

	void GameManager::Init()
	{
		_componentManager = std::make_unique<ComponentManager>();
		_gameObjectManager = std::make_unique<GameObjectManager>();
		_systemManager = std::make_unique<SystemManager>();
		_instance = this;
	}

	GameObject* GameManager::CreateGameObject(const std::string& name, Transform* parent)
	{
		return _gameObjectManager->CreateGameObject(name, parent);
	}

	void GameManager::DestroyGameObject(GameObject* gameObject)
	{
		_componentManager->GameObjectRemoved(gameObject);
		_systemManager->OnGameObjectRemoved(gameObject);
		_gameObjectManager->DestroyGameObject(gameObject);
	}

	void* GameManager::GetComponentRaw(GameObject* gameObject, ComponentType type)
	{
		return _componentManager->GetComponentRaw(gameObject, type);
	}

	void GameManager::SetScene(Scene* scene)
	{
		_scene = scene;
	}

	Scene* GameManager::GetScene()
	{
		return _scene;
	}
}