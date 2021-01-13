#include "Core/Components/ScriptedBehaviour.h"

namespace Core
{
	void ScriptedBehaviour::Init()
	{
		if (_instance._scriptObj != nullptr)
		{
			_instance.Init();
		}
	}

	void ScriptedBehaviour::Update()
	{
		if (_instance._scriptObj != nullptr)
		{
			_instance.Update();
		}
	}
}