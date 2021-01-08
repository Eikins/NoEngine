#pragma once

#include <cassert>
#include <unordered_map>

#include "Core/Systems/System.h"
#include "Core/GameObject.h"

namespace Core
{
	class SystemManager
	{
	private:
		std::unordered_map<const char*, Signature> _signatures;
		std::unordered_map<const char*, System*> _systems;
	public:
		void OnGameObjectRemoved(GameObject* gameObject);
		void OnGameObjectSignatureChanged(GameObject* gameObject);

		template<typename T>
		T* RegisterSystem()
		{
			const char* typeName = typeid(T).name();

			assert(_systems.find(typeName) == _systems.end() && "System already registered.");

			auto system = new T();
			_systems.insert({ typeName, system });
			return system;
		}

		template<typename T>
		void SetSignature(Signature signature)
		{
			const char* typeName = typeid(T).name();
			assert(_systems.find(typeName) != _systems.end() && "System not registered.");
			_signatures.insert({ typeName, signature });
		}
	};
}