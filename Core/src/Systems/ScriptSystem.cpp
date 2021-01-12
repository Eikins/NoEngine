#include "Core/Systems/ScriptSystem.h"

#include "Core/Modules/GameManager.hpp"

#include "Core/Time.h"

namespace Core
{
	Signature ScriptSystem::CreateSignature()
	{
		Signature signature;
		signature.set(_gameManager->GetComponentType<ScriptedBehaviour>());
		return signature;
	}

	void ScriptSystem::ShutdownRuntime()
	{
		_runtime.Shutdown();
	}

	void ScriptSystem::InitRuntime()
	{
		_runtime.Init("NoEngine");
	}

	void ScriptSystem::Reload()
	{
		_runtime.Reload();
	}

	void ScriptSystem::CompileAndLoadCore()
	{
		_runtime.Compile("CompiledScripts/NoEngine.dll", "Scripts/Core/Transform.cs Scripts/Core/Time.cs Scripts/Core/Script.cs");
		_runtime.LoadNoEngineAssembly("CompiledScripts/NoEngine.dll");

		_runtime.RegisterInternalCall("NoEngine.Transform::SetPosition", Scripting::ScriptInstance::Script_SetPosition);
	}

	void ScriptSystem::CompileAndLoadScripts()
	{
		auto& scripts = _gameManager->GetAllComponents<ScriptedBehaviour>();
		std::set<std::string> scriptPaths;
		std::string args = "";

		for (auto& sb : scripts)
		{
			scriptPaths.insert(sb.script->GetPath());
		}

		for (auto& path : scriptPaths)
		{
			args.append(path + " ");
		}
		args.append(" -r:CompiledScripts/NoEngine.dll");

		_runtime.Compile("CompiledScripts/Scripts.dll", args);
		_runtime.LoadAssembly("Scripts", "CompiledScripts/Scripts.dll");

		for (auto& sb : scripts)
		{
			sb._instance.SetName("Scripts", sb.script->GetName());
			_runtime.BindScript("Scripts", sb._instance);
		}
	}

	void ScriptSystem::Update()
	{
		_runtime.GetEnvironment().SetTime(Time::time, Time::deltaTime);
		auto& scriptBehaviours = _gameManager->GetAllComponents<ScriptedBehaviour>();
		for (auto& behaviour : scriptBehaviours)
		{
			if (behaviour.IsEnabled() == false || behaviour.GetGameObject()->IsActive() == false) continue;
			behaviour.Update();
		}
	}
}