#pragma once

#include "Core/Assets/Script.h"
#include "Component.h"

#include "Scripting/ScriptInstance.h"

namespace Core
{
	class ScriptedBehaviour : public Component
	{
		friend class ScriptSystem;
	private:
		std::vector<Scripting::ScriptInstance> _instances;
		std::vector<Script*> _scripts;
	public:

		ScriptedBehaviour(GameObject* gameObject) : Component(gameObject) {}

		inline std::vector<Scripting::ScriptInstance>& GetInstances() { return _instances; }
		inline std::vector<Script*>& GetScripts() { return _scripts; }

		void AddScript(Script* script);

		void Init();
		void Update();
	};
}