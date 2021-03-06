#include "Core/Systems/ScriptSystem.h"
#include "Core/Modules/GameManager.hpp"
#include "Core/Time.h"

#include "Scripting/InternalCall.h"

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
			"Scripts/Core/Input.cs",
			"Scripts/Core/Math/Math.cs",
			"Scripts/Core/Math/Vector2.cs",
			"Scripts/Core/Math/Vector3.cs",
			"Scripts/Core/Math/Quaternion.cs",
			"Scripts/Core/Components/Component.cs",
			"Scripts/Core/Components/Renderer.cs"
		};

		for (auto& path : coreScripts)
		{
			args.append(path + " ");
		}

		_runtime.Compile("CompiledScripts/NoEngine.dll", args);
		_runtime.LoadNoEngineAssembly("CompiledScripts/NoEngine.dll");

		Scripting::InternalCall::RegisterInternalCalls(_runtime);
	}

	void ScriptSystem::CompileAndLoadScripts()
	{
		auto& scripts = _gameManager->GetAllComponents<ScriptedBehaviour>();
		std::set<std::string> scriptPaths;
		std::string args = "";

		for (auto& sb : scripts)
		{
			for (auto& script : sb.GetScripts())
			{
				if (script != nullptr)
				{
					scriptPaths.insert(script->GetPath());
				}
			}
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
			auto& instances = sb.GetInstances();
			for (size_t i = 0; i < instances.size(); i++)
			{
				instances[i].SetName("Scripts", sb.GetScripts()[i]->GetName());
				_runtime.BindScript("Scripts", instances[i], sb.GetTransform());
			}
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
		_runtime.GetEnvironment().SetMousePos(InputMaster::mousePos);
		auto& scriptBehaviours = _gameManager->GetAllComponents<ScriptedBehaviour>();
		for (auto& behaviour : scriptBehaviours)
		{
			if (behaviour.IsEnabled() == false || behaviour.GetGameObject()->IsActive() == false) continue;
			behaviour.Update();
		}
	}
}