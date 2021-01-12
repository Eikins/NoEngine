#include "Core/Components/ScriptedBehaviour.h"

namespace Core
{
	void ScriptedBehaviour::Update()
	{
		if (_instance._scriptObj != nullptr)
		{
			_instance.Update();
		}
	}
}