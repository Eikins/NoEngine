#pragma once

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace Scripting
{

	class Environment
	{
		friend class Runtime;
	private:
		// Time References
		struct Time
		{
			MonoObject* obj = nullptr;
			MonoClassField* deltaTimeField = nullptr;
			MonoClassField* timeField = nullptr;
		};
	private:
		Time _time;

		void Initialize(MonoDomain* domain, MonoAssembly* assembly);
	public:


		void SetTime(float time, float deltaTime);

	};
}