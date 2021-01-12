#include "Scripting/Runtime.h"

#include <string>
#include <iostream>
#include <cstdlib>

#include <filesystem>


#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

namespace Scripting
{

	void Runtime::Init(const std::string& domainName)
	{
		mono_set_dirs(MONO_LIB_DIR, MONO_ETC_DIR);
		_domain = mono_jit_init(domainName.c_str());

		//_domain = mono_domain_create();

		if (_domain == nullptr)
		{
			throw std::runtime_error("Couldn't init a Mono Domain");
		}

		//mono_domain_set(_domain, true);
	}

	void Runtime::Reload()
	{
		auto oldDomain = _domain;
		_domain = mono_domain_create();
		mono_domain_set(_domain, true);
		//mono_domain_unload(oldDomain);
		_assemblies.clear();
	}

	void Runtime::Shutdown()
	{
		mono_jit_cleanup(_domain);
	}

	void Runtime::Compile(const std::string& output, const std::string& files)
	{
		std::string command = "\"" + std::string(MONO_BIN_DIR) + "/csc\" " + files + " -target:library -out:" + output;
		system(command.c_str());
	}

	void Runtime::LoadNoEngineAssembly(const std::string& path)
	{
		LoadAssembly("NoEngine", path);
		_mainEnvironment.Initialize(_domain, _assemblies["NoEngine"]);
	}

	void Runtime::LoadAssembly(const std::string& name, const std::string& path)
	{
		auto it = _assemblies.find(name);
		if (it == _assemblies.end())
		{
			MonoAssembly* assembly;
			assembly = mono_domain_assembly_open(_domain, path.c_str());
			if (assembly == nullptr)
			{
				throw std::runtime_error("Couldn't load the assembly");
			}
			_assemblies.insert({ name, assembly });
		}
	}

	void Runtime::UnloadAssembly(const std::string& name)
	{
		auto it = _assemblies.find(name);
		if (it != _assemblies.end())
		{
			mono_assembly_close(it->second);
			_assemblies.erase(it);
		}
	}

	void Runtime::RegisterInternalCall(const std::string& name, void* method)
	{
		mono_add_internal_call(name.c_str(), method);
	}

	void Runtime::BindScript(const std::string& assembly, ScriptInstance& script, Core::Transform* transform)
	{
		script.Bind(_domain, _assemblies[assembly], transform);
	}
}