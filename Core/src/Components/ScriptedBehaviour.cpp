#include "Core/Components/ScriptedBehaviour.h"

namespace Core
{
	void ScriptedBehaviour::Initialize()
	{
	}

	void ScriptedBehaviour::Update()
	{
		if (_instance._scriptObj != nullptr)
		{
			_instance.SetTransform(GetTransform()->GetPosition());
			_instance.Update();
			GetTransform()->SetPosition(_instance.GetPosition());
		}
	}
}