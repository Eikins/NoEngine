#pragma once

#include "Core/Assets/Script.h"
#include "Component.h"
namespace Core
{
	class ScriptedBehaviour : public Component
	{
	private:
		// TODO : Add a custom allocator to have a script specific heap
	public:
		Script* script = nullptr;

		ScriptedBehaviour(GameObject* gameObject) : Component(gameObject) {}

		void Initialize();
		void Update();
	};
}