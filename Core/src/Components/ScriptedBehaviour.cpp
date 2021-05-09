#include "Core/Components/ScriptedBehaviour.h"

namespace Core
{
	void ScriptedBehaviour::AddScript(Script* script)
	{
		_scripts.push_back(script);
		_instances.push_back(Scripting::ScriptInstance());
	}

	void ScriptedBehaviour::Init()
	{
		for (auto& instance : _instances)
		{
			if (instance._scriptObj != nullptr)
			{
				instance.Init();
			}
		}
	}

	void ScriptedBehaviour::Update()
	{
		for (auto& instance : _instances)
		{
			if (instance._scriptObj != nullptr && instance.enabled)
			{
				instance.Update();
			}
		}
	}
}