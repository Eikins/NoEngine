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
		std::string args = "";

		std::vector<std::string> coreScripts = {
			"Scripts/Core/Transform.cs",
			"Scripts/Core/Time.cs",
			"Scripts/Core/Script.cs",
			"Scripts/Core/Material.cs",
			"Scripts/Core/Components/Component.cs",
			"Scripts/Core/Components/Renderer.cs"
		};

		for (auto& path : coreScripts)
		{
			args.append(path + " ");
		}

		_runtime.Compile("CompiledScripts/NoEngine.dll", args);
		_runtime.LoadNoEngineAssembly("CompiledScripts/NoEngine.dll");

		_runtime.RegisterInternalCall("NoEngine.Script::GetComponentHandle", Scripting::ScriptInstance::Script_GetComponentHandle);

		_runtime.RegisterInternalCall("NoEngine.Renderer::GetMaterial", Scripting::ScriptInstance::Script_GetMaterial);
		_runtime.RegisterInternalCall("NoEngine.Material::SetColor", Scripting::ScriptInstance::Script_SetColor);

		_runtime.RegisterInternalCall("NoEngine.Transform::SetPosition", Scripting::ScriptInstance::Script_SetPosition);
		_runtime.RegisterInternalCall("NoEngine.Transform::GetPosition", Scripting::ScriptInstance::Script_GetPosition);
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
			_runtime.BindScript("Scripts", sb._instance, sb.GetTransform());
		}
	}

	void ScriptSystem::ScriptInit()
	{
		auto& scriptBehaviours = _gameManager->GetAllComponents<ScriptedBehaviour>();
		for (auto& behaviour : scriptBehaviours)
		{
			behaviour.Init();
		}
	}

	void ScriptSystem::ScriptUpdate()
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