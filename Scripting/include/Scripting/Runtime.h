#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "mono/jit/jit.h"

#include "Environment.h"
#include "ScriptInstance.h"

#include "Core/Transform.h"

namespace Scripting
{
	class Runtime
	{
		friend class Environment;
		friend class ScriptInstance;
	private:
		MonoDomain* _domain;
		std::unordered_map<std::string, MonoAssembly*> _assemblies;

		Environment _mainEnvironment;
	public:
		void Init(const std::string& domainName);
		void Compile(const std::string& output, const std::string& files);
		void Shutdown();

		void Reload();

		void LoadNoEngineAssembly(const std::string& path);
		void LoadAssembly(const std::string& name, const std::string& path);
		void UnloadAssembly(const std::string& name);

		void RegisterInternalCall(const std::string& name, void* method);
		void BindScript(const std::string& assembly, ScriptInstance& script, Core::Transform* transform);

		inline Environment& GetEnvironment() { return _mainEnvironment; }
	};
}