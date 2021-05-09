#pragma once

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

#include "Core/Input/InputMaster.h"

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
		struct InputEvent
		{
			MonoClass* inputEventClass = nullptr;
			MonoObject* obj = nullptr;
			MonoClassField* mousePosField = nullptr;
		};
	private:
		Runtime* _runtime;
		Time _time;
		InputEvent _input;

		void Initialize(Runtime* runtime, MonoAssembly* assembly);
	public:
		MonoObject* BoxInputEvent(const Core::InputEvent& event);

		void SetTime(float time, float deltaTime);
		void SetMousePos(Math::Vector2 position);
	};
}