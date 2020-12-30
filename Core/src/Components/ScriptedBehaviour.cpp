#include "Core/Components/ScriptedBehaviour.h"

namespace Core
{
	void ScriptedBehaviour::Initialize()
	{
		if (script != nullptr)
		{
			script->Initialize(GetGameObject(), GetTransform());
		}
	}

	void ScriptedBehaviour::Update()
	{
		if (script != nullptr)
		{
			script->OnUpdate(GetGameObject(), GetTransform());
		}
	}
}