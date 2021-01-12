#pragma once

#include <memory>

#include "ComponentManager.hpp"
#include "GameObjectManager.h"
#include "SystemManager.hpp"

namespace Core
{
	class GameManager
	{
	private:
		std::unique_ptr<ComponentManager> _componentManager;
		std::unique_ptr<GameObjectManager> _gameObjectManager;
		std::unique_ptr<SystemManager> _systemManager;

		Scene* _scene;
	public:
		void Init();
		GameObject* CreateGameObject(const std::string& name, Transform* parent = nullptr);
		void DestroyGameObject(GameObject* gameObject);

		void SetScene(Scene* scene);
		Scene* GetScene();

		template<typename T>
		void RegisterComponent()
		{
			_componentManager->RegisterComponent<T>();
		}

		template<typename T>
		T* RegisterSystem()
		{
			auto ptr = _systemManager->RegisterSystem<T>();
			ptr->_gameManager = this;
			return ptr;
		}


		template<typename T>
		T* GetSystem()
		{
			return _systemManager->GetSystem<T>();
		}

		template<typename T>
		void SetSystemSignature(Signature signature)
		{
			_systemManager->SetSignature<T>(signature);
		}

		template<typename T>
		T& AddComponent(GameObject* gameObject)
		{
			T& comp = _componentManager->AddComponent<T>(gameObject);
			// Update go signature (equivalent to comp list)
			gameObject->_signature.set(_componentManager->GetType<T>(), true);
			_systemManager->OnGameObjectSignatureChanged(gameObject);
			return comp;
		}

		template<typename T>
		void RemoveComponent(GameObject* gameObject)
		{
			_componentManager->RemoveComponent<T>(entity);

			gameObject->_signature.set(_componentManager->GetType<T>(), false);
			_systemManager->OnGameObjectSignatureChanged(gameObject);
		}

		template<typename T>
		T* GetComponent(GameObject* gameObject)
		{
			return _componentManager->GetComponent<T>(gameObject);
		}

		template<typename T>
		ComponentType GetComponentType()
		{
			return _componentManager->GetType<T>();
		}

		template<typename T>
		std::vector<T>& GetAllComponents()
		{
			return _componentManager->GetAllComponents<T>();
		}
	};
}