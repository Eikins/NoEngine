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
		Scripting::ScriptInstance _instance;
	public:
		Script* script = nullptr;

		ScriptedBehaviour(GameObject* gameObject) : Component(gameObject) {}

		inline Scripting::ScriptInstance& GetInstance() { return _instance; }

		void Initialize();
		void Update();
	};
}