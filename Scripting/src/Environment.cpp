#include "Scripting/Environment.h"

#include "Scripting/Runtime.h"

namespace Scripting
{

	void Environment::Initialize(Runtime* runtime, MonoAssembly* assembly)
	{
		_runtime = runtime;
		MonoImage* image;
		image = mono_assembly_get_image(assembly);

		// Initialize Time Class
		{
			auto timeClass = mono_class_from_name(image, "NoEngine", "Time");
			_time.obj = mono_object_new(runtime->_domain, timeClass);
			_time.deltaTimeField = mono_class_get_field_from_name(timeClass, "deltaTime");
			_time.timeField = mono_class_get_field_from_name(timeClass, "time");
			mono_runtime_object_init(_time.obj);
		}

		// Initialize InputEvent
		{
			_input.inputEventClass = mono_class_from_name(image, "NoEngine", "InputEvent");
			auto inputClass = mono_class_from_name(image, "NoEngine", "Input");
			_input.obj = mono_object_new(runtime->_domain, inputClass);
			_input.mousePosField = mono_class_get_field_from_name(inputClass, "mousePos");
			mono_runtime_object_init(_input.obj);
		}
	}

	void Environment::SetTime(float time, float deltaTime)
	{
		mono_field_static_set_value(_time.obj->vtable, _time.timeField, &time);
		mono_field_static_set_value(_time.obj->vtable, _time.deltaTimeField, &deltaTime);
	}

	void Environment::SetMousePos(Math::Vector2 position)
	{
		mono_field_static_set_value(_input.obj->vtable, _input.mousePosField, &position);
	}

	MonoObject* Environment::BoxInputEvent(const Core::InputEvent& event)
	{
		auto value = event;
		return mono_value_box(_runtime->_domain, _input.inputEventClass, &value);
	}
}